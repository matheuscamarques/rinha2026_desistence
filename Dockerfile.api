# Stage 1: Build da API e do Healthcheck
FROM --platform=linux/amd64 gcc:13 AS builder

WORKDIR /src

# Instala dependências de build (git para clonar yyjson/facil.io)
RUN set -eux; \
    apt-get update && apt-get install -y --no-install-recommends git && \
    rm -rf /var/lib/apt/lists/*

# Copia apenas o necessário para o build
# Certifique-se de que lib/model_infer_lgb_v2.c foi gerado antes do docker build
COPY Makefile ./
COPY main.c ./
COPY healthcheck.c ./
COPY lib/ ./lib/
COPY priv/mcc_risk.json ./priv/mcc_risk.json

# Compila as dependências e o binário principal
RUN set -eux; \
    make deps; \
    make -j"$(nproc)"; \
    # Compila um healthcheck minimalista
    cc -O2 -s healthcheck.c -o healthcheck; \
    # Remove símbolos desnecessários para diminuir o binário
    strip -s ./fraud_api

# Stage 2: Runtime minimalista
FROM --platform=linux/amd64 debian:12-slim AS runtime

# Otimização de memória para aplicações C de alta concorrência
ENV MALLOC_ARENA_MAX=2

WORKDIR /app

# Copia apenas os binários e os arquivos de configuração necessários
COPY --from=builder /src/fraud_api ./fraud_api
COPY --from=builder /src/healthcheck ./healthcheck
COPY --from=builder /src/priv/mcc_risk.json ./priv/mcc_risk.json

# Expondo a porta configurada no main.c[cite: 15]
EXPOSE 9999

# Inicia a API
CMD ["./fraud_api"]