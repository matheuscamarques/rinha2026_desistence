#!/usr/bin/env python3
import sys
import re
from pathlib import Path
import tempfile
import treelite
import tl2cgen

def main():
    model_path = Path("models/lgb_model_v2.txt")
    output_path = Path("lib/model_infer_lgb_v2.c")
    
    if not model_path.exists():
        print(f"Error: {model_path} not found.")
        sys.exit(1)
    
    print(f"Loading LightGBM model...")
    model_tl = treelite.frontend.load_lightgbm_model(str(model_path))
    
    with tempfile.TemporaryDirectory() as tmpdir:
        tmpdir_path = Path(tmpdir)
        # parallel_comp=1 gera um arquivo C único e limpo
        tl2cgen.generate_c_code(model_tl, str(tmpdir_path), params={'parallel_comp': 1})
        
        src_files = list(tmpdir_path.glob("*.c"))
        predict_src = ""
        for src_file in src_files:
            content = src_file.read_text()
            # Remove dependências de headers externos e duplicados
            content = re.sub(r'#include\s+".*header.h"', '', content)
            predict_src += content

    # Cabeçalho auto-contido com definições de hardware
    header = '''/* Auto-generated Optimized C Code - Rinha 2026 */
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

#define N_TARGET 1
#define MAX_N_CLASS 1

#ifndef LIKELY
#ifdef __GNUC__
#define LIKELY(x)      __builtin_expect(!!(x), 1)
#define UNLIKELY(x)    __builtin_expect(!!(x), 0)
#else
#define LIKELY(x)      (x)
#define UNLIKELY(x)    (x)
#endif
#endif

/* Estrutura fundamental para modelos float64 */
union Entry {
  int32_t missing;
  double fvalue;
  int32_t qvalue;
};

void predict_unit0(union Entry* data, double* result);
void postprocess(double* result);
void predict(union Entry* data, int pred_margin, double* result);
'''

    # WRAPPER CORRIGIDO: Usa double e inicializa out_result
    wrapper = '''
/* Wrapper para o vetor de 19 features do main.c */
static inline double predict_fraud_lgb(const double *features) {
    union Entry input[19];
    for (int i = 0; i < 19; i++) {
        input[i].fvalue = features[i]; // Sem cast para float!
        input[i].missing = 0;          // 0 indica que o valor está presente
    }
    
    // CRÍTICO: Inicializar com 0.0 pois o modelo faz acumulação
    double out_result[1] = {0.0}; 
    
    predict(input, 0, out_result); 
    return out_result[0];
}
'''
    
    with open(output_path, 'w') as f:
        f.write(header + predict_src + wrapper)
    
    print(f"✓ Modelo exportado e Wrapper blindado em {output_path}")

if __name__ == '__main__':
    main()