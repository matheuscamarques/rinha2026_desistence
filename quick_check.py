#!/usr/bin/env python3
"""
Quick Check: Valida métricas de um par de features específico
Uso: python3 quick_check.py F8 F12 [arquivo_dados]
"""

import json
import gzip
import sys
import numpy as np
from pathlib import Path

def load_data(filepath, max_samples=100_000):
    """Carrega dados de references.json.gz."""
    data = []
    try:
        with gzip.open(filepath, 'rt') as f:
            line_idx = 0
            for line in f:
                items = line.strip().split('},{')
                for item in items:
                    item = item.strip()
                    if not item.startswith('{'):
                        item = '{' + item
                    if not item.endswith('}'):
                        item = item + '}'
                    
                    try:
                        obj = json.loads(item)
                        if 'vector' in obj:
                            vector = np.clip(np.array(obj['vector'], dtype=float) * 255, 0, 255)
                            data.append(vector)
                            line_idx += 1
                            
                            if line_idx % 50_000 == 0:
                                print(f"  {line_idx} registros carregados...")
                            
                            if max_samples and line_idx >= max_samples:
                                return np.array(data, dtype=np.float32)
                    except:
                        continue
                
                if max_samples and line_idx >= max_samples:
                    break
    except Exception as e:
        print(f"Erro ao carregar: {e}")
        return None
    
    if data:
        print(f"  ✓ Total: {len(data)} registros carregados")
        return np.array(data, dtype=np.float32)
    return None

def quick_check(f1_idx, f2_idx, data):
    """Calcula métricas rápidas para um par."""
    print(f"\n╔{'═' * 70}╗")
    print(f"║ QUICK CHECK: F{f1_idx} + F{f2_idx}")
    print(f"╚{'═' * 70}╝\n")
    
    if f1_idx >= data.shape[1] or f2_idx >= data.shape[1]:
        print(f"❌ Features inválidas. Dataset tem {data.shape[1]} features.")
        return
    
    f1 = data[:, f1_idx]
    f2 = data[:, f2_idx]
    
    # Estatísticas
    print("📊 Estatísticas Individuais:")
    print(f"  F{f1_idx}: µ={f1.mean():.2f}, σ={f1.std():.2f}, range=[{f1.min():.0f}, {f1.max():.0f}]")
    print(f"  F{f2_idx}: µ={f2.mean():.2f}, σ={f2.std():.2f}, range=[{f2.min():.0f}, {f2.max():.0f}]")
    
    # Sparsity
    zeros_f1 = np.sum(f1 == 0) / len(f1)
    zeros_f2 = np.sum(f2 == 0) / len(f2)
    print(f"\n🔍 Sparsity (Fração de Zeros):")
    print(f"  F{f1_idx}: {zeros_f1:.1%}", end="")
    if zeros_f1 > 0.1:
        print(" 🔴 CRÍTICO (>10%)")
    elif zeros_f1 > 0.05:
        print(" 🟠 ALTO (>5%)")
    else:
        print(" ✅ OK")
    
    print(f"  F{f2_idx}: {zeros_f2:.1%}", end="")
    if zeros_f2 > 0.1:
        print(" 🔴 CRÍTICO (>10%)")
    elif zeros_f2 > 0.05:
        print(" 🟠 ALTO (>5%)")
    else:
        print(" ✅ OK")
    
    # Correlação
    corr = np.corrcoef(f1, f2)[0, 1]
    print(f"\n🔗 Correlação de Pearson:")
    print(f"  |r| = {abs(corr):.4f}", end="")
    if abs(corr) < 0.1:
        print(" ✅ ORTOGONAL")
    elif abs(corr) < 0.3:
        print(" 🟢 BOM")
    elif abs(corr) < 0.5:
        print(" 🟡 MÉDIO")
    else:
        print(" 🔴 RUIM (correlacionado)")
    
    # Fitness Score
    sparse_penalty = (zeros_f1 + zeros_f2) / 2
    fitness = (f1.std() * f2.std()) / (abs(corr) + 0.1) * (1 - sparse_penalty)
    print(f"\n⚡ Fitness Score:")
    print(f"  Score = {fitness:,.0f}", end="")
    if fitness > 50_000:
        print(" 🌟 EXCELENTE")
    elif fitness > 40_000:
        print(" ✅ BOM")
    elif fitness > 30_000:
        print(" 🟡 ACEITÁVEL")
    else:
        print(" 🔴 BAIXO")
    
    # Recomendação
    print(f"\n🎯 Recomendação:")
    if sparse_penalty > 0.15 + max(zeros_f1, zeros_f2):
        print(f"  ❌ NÃO USE (sparsity = {sparse_penalty:.1%})")
    elif abs(corr) > 0.5:
        print(f"  ⚠️  EVITE (correlados: r={corr:.3f})")
    else:
        print(f"  ✅ VIÁVEL (Fitness: {fitness:,.0f})")
    
    print(f"\n{'═' * 70}\n")

def main():
    if len(sys.argv) < 3:
        print("Uso: python3 quick_check.py <F1> <F2> [caminho_dados]")
        print("Exemplo: python3 quick_check.py 8 12")
        sys.exit(1)
    
    try:
        f1_idx = int(sys.argv[1])
        f2_idx = int(sys.argv[2])
    except ValueError:
        print(f"❌ Features devem ser índices numéricos (0-13)")
        sys.exit(1)
    
    # Caminho dos dados
    data_path = Path(sys.argv[3]) if len(sys.argv) > 3 else Path(
        "documentations/rinha-de-backend-2026/resources/references.json.gz"
    )
    
    if not data_path.exists():
        print(f"❌ Arquivo não encontrado: {data_path}")
        print("[*] Gerando dados sintéticos para demo...")
        # Dados sintéticos
        np.random.seed(42)
        data = np.random.uniform(0, 255, (100_000, 14)).astype(np.float32)
        # Torna F0 e F6 correlacionadas (teste com F0+F6)
        data[:, 6] = data[:, 0] * 0.8 + np.random.randn(100_000) * 20
        data = np.clip(data, 0, 255)
    else:
        print(f"[*] Carregando {data_path}...")
        data = load_data(data_path, max_samples=100_000)
        if data is None:
            sys.exit(1)
    
    quick_check(f1_idx, f2_idx, data)

if __name__ == "__main__":
    main()
