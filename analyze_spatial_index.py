#!/usr/bin/env python3
"""
Análise de Seleção de Features para Spatial Index 2D (256×256)
Motor KNN em C | 3M registros | uint8_t quantizados (0-255)
Hardware: Mac Mini 2014 | CPU cap: 0.4 | Docker
"""

import json
import gzip
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from pathlib import Path
import warnings

warnings.filterwarnings('ignore')

# ============================================================================
# CONFIGURAÇÃO
# ============================================================================
VECTOR_DIM = 14  # References.bin contains 14 features (raw input)
# Feature engineering to 19 features happens in main.c during inference
SAMPLE_SIZE = 100_000  # Limita a memória para análise rápida
SENTINEL_VALUE = -1  # Valor sentinela para dados faltantes
DATA_PATH = Path("documentations/rinha-de-backend-2026/resources/references.json.gz")
OUTPUT_DIR = Path("analysis_output")

# Cria diretório de saída
OUTPUT_DIR.mkdir(exist_ok=True)

# Configuração visual
sns.set_theme(style="whitegrid", palette="husl")
plt.rcParams['figure.facecolor'] = '#f8f9fa'
plt.rcParams['font.size'] = 10

# ============================================================================
# CARREGAMENTO DE DADOS
# ============================================================================
def load_references(path, max_samples=None):
    """Carrega dados de references.json.gz como DataFrame."""
    print(f"[*] Carregando dados de {path}...")
    
    if not path.exists():
        print(f"[!] Arquivo não encontrado: {path}")
        print("[*] Gerando dados sintéticos para demo...")
        return generate_synthetic_data(max_samples or SAMPLE_SIZE)
    
    data = []
    try:
        with gzip.open(path, 'rt') as f:
            line_idx = 0
            for line in f:
                # Cada linha contém múltiplos objetos JSON concatenados com },{ separador
                items = line.strip().split('},{')
                for item in items:
                    item = item.strip()
                    if not item.startswith('{'):
                        item = '{' + item
                    if not item.endswith('}'):
                        item = item + '}'
                    
                    try:
                        obj = json.loads(item)
                    except json.JSONDecodeError:
                        continue
                    
                    if max_samples and line_idx >= max_samples:
                        if data:
                            df = pd.DataFrame(data, columns=[f'F{i}' for i in range(VECTOR_DIM)])
                            print(f"[+] {len(df)} registros carregados. Shape: {df.shape}")
                            return df
                        break
                    
                    # Extrai features do campo 'vector'
                    if 'vector' in obj:
                        features = np.array(obj['vector'], dtype=np.float32)
                        
                        # Quantiza valores para [0, 255]
                        # Assume que dados estão em [0, 1] e comporta sentinelas (-1)
                        features = np.where(features < 0, 0, features)  # Sentinelas viram 0
                        features = np.clip(features * 255, 0, 255).astype(np.uint8)
                        
                        data.append(features)
                        line_idx += 1
                        
                        if line_idx % 50_000 == 0:
                            print(f"  [.] {line_idx} registros carregados...")
                    
                if max_samples and line_idx >= max_samples:
                    break
                    
    except Exception as e:
        print(f"[!] Erro ao carregar: {e}")
        if data:
            print(f"[*] Usando {len(data)} registros carregados antes do erro...")
            df = pd.DataFrame(data, columns=[f'F{i}' for i in range(VECTOR_DIM)])
            print(f"[+] {len(df)} registros. Shape: {df.shape}")
            return df
        else:
            print("[*] Usando dados sintéticos...")
            return generate_synthetic_data(max_samples or SAMPLE_SIZE)
    
    if not data:
        print("[!] Nenhum dado encontrado. Usando dados sintéticos...")
        return generate_synthetic_data(max_samples or SAMPLE_SIZE)
    
    df = pd.DataFrame(data, columns=[f'F{i}' for i in range(VECTOR_DIM)])
    print(f"[+] {len(df)} registros carregados. Shape: {df.shape}")
    return df

def generate_synthetic_data(n_samples):
    """Gera dados sintéticos que simulam o padrão esperado."""
    print(f"[*] Gerando {n_samples} registros sintéticos...")
    
    # Simula diferentes distribuições por feature
    np.random.seed(42)
    data = {}
    
    # Features com distribuições diferentes (alguma correlação natural)
    data['F0'] = np.random.beta(2, 5, n_samples) * 255  # Skewed left
    data['F1'] = np.random.beta(5, 2, n_samples) * 255  # Skewed right
    data['F2'] = np.random.normal(128, 30, n_samples)   # Normal centered
    data['F3'] = np.random.uniform(0, 255, n_samples)   # Uniform
    data['F4'] = np.random.exponential(50, n_samples)   # Exponential
    data['F5'] = np.concatenate([
        np.zeros(int(n_samples * 0.3)),
        np.random.uniform(0, 255, int(n_samples * 0.7))
    ])  # Sparse (30% zeros)
    
    # Correladas com F0 (teste de independência)
    data['F6'] = data['F0'] * 0.8 + np.random.normal(0, 20, n_samples)
    
    # Mais features ortogonais
    data['F7'] = np.random.laplace(100, 40, n_samples)
    data['F8'] = np.random.uniform(0, 255, n_samples)
    data['F9'] = np.random.normal(64, 50, n_samples)
    data['F10'] = np.random.exponential(80, n_samples)
    data['F11'] = np.random.beta(3, 3, n_samples) * 255
    data['F12'] = np.random.uniform(0, 255, n_samples)
    data['F13'] = np.random.normal(200, 30, n_samples)
    
    # Clipa valores ao range [0, 255]
    df = pd.DataFrame(data)
    df = df.clip(0, 255).fillna(0).astype(np.uint8)
    
    print(f"[+] {len(df)} registros sintéticos gerados. Shape: {df.shape}")
    return df

# ============================================================================
# ANÁLISE DE SPARSITY (HOTSPOTS)
# ============================================================================
def analyze_sparsity(df):
    """Analisa fração de zeros e sentinelas por feature."""
    print("\n[*] Analisando Sparsity (Hotspots)...")
    
    sparsity = {}
    for col in df.columns:
        zeros = (df[col] == 0).sum()
        zero_frac = zeros / len(df)
        
        # Calcula estatísticas de densidade
        sparsity[col] = {
            'zeros_fraction': zero_frac,
            'sentinel_fraction': 0,  # Já removidas durante carregamento
            'total_sparse': zero_frac,
            'unique_values': df[col].nunique(),
            'data_density': 1 - zero_frac
        }
    
    sparsity_df = pd.DataFrame(sparsity).T
    sparsity_df = sparsity_df.sort_values('total_sparse', ascending=False)
    
    print("\n[+] Análise de Sparsity:")
    print(sparsity_df.to_string())
    
    # Salva resultado
    sparsity_df.to_csv(OUTPUT_DIR / "sparsity_analysis.csv")
    
    return sparsity_df

# ============================================================================
# RANKING DE VARIABILIDADE (STDDEV)
# ============================================================================
def analyze_variability(df):
    """Calcula stddev e range para cada feature."""
    print("\n[*] Analisando Variabilidade (StdDev)...")
    
    stats = pd.DataFrame({
        'mean': df.mean(),
        'std': df.std(),
        'min': df.min(),
        'max': df.max(),
        'range': df.max() - df.min(),
        'cv': df.std() / (df.mean() + 1)  # Coeficiente de variação (evita divisão por zero)
    })
    stats = stats.sort_values('std', ascending=False)
    
    print("\n[+] Ranking de Variabilidade:")
    print(stats.to_string())
    
    stats.to_csv(OUTPUT_DIR / "variability_analysis.csv")
    
    return stats

# ============================================================================
# CORRELAÇÃO DE PEARSON
# ============================================================================
def analyze_correlation(df):
    """Computa matriz de correlação e identifica pares ortogonais."""
    print("\n[*] Analisando Correlação de Pearson...")
    
    corr = df.corr()
    
    print("\n[+] Matriz de Correlação (resumo):")
    print(corr.round(3).to_string())
    
    corr.to_csv(OUTPUT_DIR / "correlation_matrix.csv")
    
    return corr

# ============================================================================
# CÁLCULO DE FITNESS SCORE
# ============================================================================
def calculate_fitness_scores(df, stats, corr, sparsity_df):
    """
    Calcula Fitness Score para cada par de features.
    Fórmula: (std_x * std_y) / (|corr| + 0.1) * (1 - sparse_penalty)
    """
    print("\n[*] Calculando Fitness Scores...")
    
    fitness = []
    features = list(df.columns)
    
    for i, f1 in enumerate(features):
        for j, f2 in enumerate(features):
            if i >= j:  # Evita duplicatas e auto-pares
                continue
            
            std_x = stats.loc[f1, 'std']
            std_y = stats.loc[f2, 'std']
            correlation = abs(corr.loc[f1, f2])
            
            # Penalidade por sparsity
            sparse_x = sparsity_df.loc[f1, 'total_sparse']
            sparse_y = sparsity_df.loc[f2, 'total_sparse']
            sparse_penalty = (sparse_x + sparse_y) / 2
            
            # Fitness score
            fitness_score = (std_x * std_y) / (correlation + 0.1) * (1 - sparse_penalty)
            
            fitness.append({
                'Pair': f'{f1}+{f2}',
                'F1': f1,
                'F2': f2,
                'Std_X': std_x,
                'Std_Y': std_y,
                'Correlation': correlation,
                'Data_Density_X': sparsity_df.loc[f1, 'data_density'],
                'Data_Density_Y': sparsity_df.loc[f2, 'data_density'],
                'Sparse_Penalty': sparse_penalty,
                'Fitness_Score': fitness_score
            })
    
    fitness_df = pd.DataFrame(fitness)
    fitness_df = fitness_df.sort_values('Fitness_Score', ascending=False)
    
    print("\n[+] Top 10 Pares (Fitness Score):")
    print(fitness_df.head(10)[['Pair', 'Correlation', 'Sparse_Penalty', 'Fitness_Score']].to_string(index=False))
    
    fitness_df.to_csv(OUTPUT_DIR / "fitness_scores.csv", index=False)
    
    return fitness_df

# ============================================================================
# VISUALIZAÇÕES
# ============================================================================
def plot_sparsity_analysis(sparsity_df):
    """Plota fração de zeros e sentinelas por feature."""
    print("\n[*] Plotando análise de Sparsity...")
    
    fig, axes = plt.subplots(1, 2, figsize=(14, 5))
    
    # Barras de zeros
    sparsity_df['zeros_fraction'].sort_values(ascending=False).plot(
        kind='barh', ax=axes[0], color='#e74c3c'
    )
    axes[0].set_xlabel('Fração de Zeros')
    axes[0].set_title('Hotspots: Fração de Zeros por Feature')
    axes[0].axvline(0.1, color='orange', linestyle='--', label='Threshold 10%')
    axes[0].legend()
    
    # Barras de data density
    sparsity_df['data_density'].sort_values(ascending=True).plot(
        kind='barh', ax=axes[1], color='#3498db'
    )
    axes[1].set_xlabel('Data Density (1 - Sparse)')
    axes[1].set_title('Densidade de Dados por Feature')
    axes[1].axvline(0.8, color='green', linestyle='--', label='Target 80%+')
    axes[1].legend()
    
    plt.tight_layout()
    plt.savefig(OUTPUT_DIR / "01_sparsity_analysis.png", dpi=150, bbox_inches='tight')
    print(f"[+] Salvo: 01_sparsity_analysis.png")
    plt.close()

def plot_variability_ranking(stats):
    """Plota ranking de variabilidade (StdDev)."""
    print("[*] Plotando ranking de variabilidade...")
    
    fig, axes = plt.subplots(1, 2, figsize=(14, 5))
    
    # StdDev ranking
    stats['std'].sort_values(ascending=False).plot(
        kind='barh', ax=axes[0], color='#2ecc71'
    )
    axes[0].set_xlabel('Desvio Padrão')
    axes[0].set_title('Ranking: Variabilidade por Feature')
    
    # Range (max - min)
    stats['range'].sort_values(ascending=False).plot(
        kind='barh', ax=axes[1], color='#f39c12'
    )
    axes[1].set_xlabel('Range (Max - Min)')
    axes[1].set_title('Ranking: Span de Dados por Feature')
    
    plt.tight_layout()
    plt.savefig(OUTPUT_DIR / "02_variability_ranking.png", dpi=150, bbox_inches='tight')
    print(f"[+] Salvo: 02_variability_ranking.png")
    plt.close()

def plot_correlation_matrix(corr):
    """Plota heatmap de correlação."""
    print("[*] Plotando matriz de correlação...")
    
    fig, ax = plt.subplots(figsize=(12, 10))
    
    sns.heatmap(
        corr, 
        annot=True, 
        fmt='.2f', 
        cmap='coolwarm', 
        center=0,
        square=True,
        linewidths=0.5,
        cbar_kws={'label': 'Correlação de Pearson'},
        ax=ax
    )
    ax.set_title('Matriz de Correlação: 14 Features')
    
    plt.tight_layout()
    plt.savefig(OUTPUT_DIR / "03_correlation_matrix.png", dpi=150, bbox_inches='tight')
    print(f"[+] Salvo: 03_correlation_matrix.png")
    plt.close()

def plot_fitness_scores(fitness_df):
    """Plota top pares por Fitness Score."""
    print("[*] Plotando Fitness Scores...")
    
    top_pairs = fitness_df.head(15)
    
    fig, axes = plt.subplots(1, 2, figsize=(14, 6))
    
    # Fitness Score
    top_pairs.sort_values('Fitness_Score', ascending=True).plot(
        x='Pair', y='Fitness_Score', kind='barh', ax=axes[0], 
        color='#9b59b6', legend=False
    )
    axes[0].set_xlabel('Fitness Score')
    axes[0].set_title('Top 15 Pares: Fitness Score')
    axes[0].tick_params(axis='y', labelsize=8)
    
    # Scatter: Correlação vs Sparse Penalty
    axes[1].scatter(
        top_pairs['Correlation'], 
        top_pairs['Sparse_Penalty'],
        s=top_pairs['Fitness_Score'] * 10,
        alpha=0.6,
        c=top_pairs.index,
        cmap='viridis'
    )
    for idx, row in top_pairs.iterrows():
        axes[1].annotate(
            row['Pair'], 
            (row['Correlation'], row['Sparse_Penalty']),
            fontsize=7, 
            alpha=0.7
        )
    axes[1].set_xlabel('Correlação (abs)')
    axes[1].set_ylabel('Penalidade de Sparsity')
    axes[1].set_title('Trade-off: Independência vs Densidade')
    axes[1].grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig(OUTPUT_DIR / "04_fitness_scores.png", dpi=150, bbox_inches='tight')
    print(f"[+] Salvo: 04_fitness_scores.png")
    plt.close()

def plot_2d_distributions(df, fitness_df):
    """Plota distribuições 2D (hexbin) para top 4 pares."""
    print("[*] Plotando distribuições 2D (Hexbin)...")
    
    top_pairs = fitness_df.head(4)
    
    fig, axes = plt.subplots(2, 2, figsize=(12, 10))
    axes = axes.flatten()
    
    for idx, (ax_idx, (_, row)) in enumerate(zip(range(4), top_pairs.iterrows())):
        f1, f2 = row['F1'], row['F2']
        
        # Hexbin plot
        hb = axes[ax_idx].hexbin(
            df[f1], df[f2],
            gridsize=30,
            cmap='YlOrRd',
            mincnt=1
        )
        axes[ax_idx].set_xlabel(f'{f1}')
        axes[ax_idx].set_ylabel(f'{f2}')
        axes[ax_idx].set_title(
            f'{row["Pair"]}\n(Fit: {row["Fitness_Score"]:.1f}, Corr: {row["Correlation"]:.3f})'
        )
        plt.colorbar(hb, ax=axes[ax_idx], label='Densidade')
    
    plt.tight_layout()
    plt.savefig(OUTPUT_DIR / "05_2d_distributions.png", dpi=150, bbox_inches='tight')
    print(f"[+] Salvo: 05_2d_distributions.png")
    plt.close()

# ============================================================================
# MAIN
# ============================================================================
def main():
    print("=" * 80)
    print("ANÁLISE DE SPATIAL INDEX 2D | KNN Motor")
    print("=" * 80)
    
    # Carrega dados
    df = load_references(DATA_PATH, SAMPLE_SIZE)
    
    # Análises
    sparsity_df = analyze_sparsity(df)
    stats = analyze_variability(df)
    corr = analyze_correlation(df)
    fitness_df = calculate_fitness_scores(df, stats, corr, sparsity_df)
    
    # Visualizações
    plot_sparsity_analysis(sparsity_df)
    plot_variability_ranking(stats)
    plot_correlation_matrix(corr)
    plot_fitness_scores(fitness_df)
    plot_2d_distributions(df, fitness_df)
    
    # Resumo
    print("\n" + "=" * 80)
    print("RECOMENDAÇÕES")
    print("=" * 80)
    
    best_pair = fitness_df.iloc[0]
    print(f"\n[★] MELHOR PAR (Fitness Score): {best_pair['Pair']}")
    print(f"    Std_X={best_pair['Std_X']:.2f}, Std_Y={best_pair['Std_Y']:.2f}")
    print(f"    Correlação: {best_pair['Correlation']:.3f} (independência: {'✓' if best_pair['Correlation'] < 0.3 else '✗'})")
    print(f"    Densidade: {best_pair['Data_Density_X']:.1%} x {best_pair['Data_Density_Y']:.1%}")
    print(f"    Fitness Score: {best_pair['Fitness_Score']:.1f}")
    
    print(f"\n[★] TOP 5 PARES:")
    for i, (_, row) in enumerate(fitness_df.head(5).iterrows(), 1):
        print(f"  {i}. {row['Pair']:8s} (Fit: {row['Fitness_Score']:7.1f}, Corr: {row['Correlation']:.3f})")
    
    print(f"\n[!] FEATURES COM ALTA SPARSITY (>10% zeros):")
    sparse_features = sparsity_df[sparsity_df['zeros_fraction'] > 0.1]
    if not sparse_features.empty:
        for feat, row in sparse_features.iterrows():
            print(f"  - {feat}: {row['zeros_fraction']:.1%} zeros (EVITAR)")
    else:
        print("  Nenhuma feature crítica identificada ✓")
    
    print(f"\n[+] Relatórios salvos em: {OUTPUT_DIR}/")
    print("=" * 80 + "\n")

if __name__ == "__main__":
    main()
