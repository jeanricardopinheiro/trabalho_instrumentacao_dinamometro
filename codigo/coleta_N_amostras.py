importimportimportimportimportserial
struct
csv
sys
time
def iniciar_coleta_N():
"""
Script Python para se conectar a um Arduino em streaming,
sincronizar com START_FLAG (0xAA) e END_FLAG (0x55) e salvar N
amostras.
"""
# --- 1. Configuração do Usuário ---
print("Iniciando script de aquisição (N Amostras) com START/END
Flags...")
try:
port = input("Digite a porta COM (ex: COM3): ")
baud = 115200 # Deve ser igual ao do C
# Fs é usada APENAS para criar o vetor de tempo no CSV
Fs = float(input("Digite a Frequência de Amostragem (Fs) em Hz
(ex: 1000): "))
# N_TOTAL é o contador de amostras que o Python irá salvar
N_TOTAL = int(input("Digite o Número Total de Amostras a coletar
(ex: 500): "))
filename = input("Digite o nome do arquivo de saída (ex:
dados_filtrados.csv): ")
except ValueError:
print("Entrada inválida.")
return
# --- 2. Cálculos Iniciais ---
dt = 1.0 / Fs
 # Período
# Formato dos dados (do C): 1 float de 4 bytes (out)
DATA_FORMAT = '<f' # '<' = Little-Endian (AVR), 'f' = float (4 bytes)
DATA_SIZE = struct.calcsize(DATA_FORMAT) # 4 bytes
# --- DEFINIÇÃO DAS FLAGS (Devem bater com o C) ---
START_FLAG = b'\xAA' # 10101010
END_FLAG
 = b'\x55' # 01010101
ser = None
csv_file = None
print(f"\nIniciando... Coletando {N_TOTAL} amostras.")
print(f"Procurando START_FLAG ({START_FLAG.hex().upper()})...")
try:
# --- 3. Abrir Conexão Serial e Arquivo CSV ---
ser = serial.Serial(port, baud, timeout=2)
csv_file = open(filename, 'w', newline='')
csv_writer = csv.writer(csv_file)
# Escreve o cabeçalho no CSV
csv_writer.writerow(['Tempo (s)', 'Sinal Filtrado (V)'])
time.sleep(1) # Aguarda o Arduino (se houver auto-reset)
ser.reset_input_buffer() # Limpa qualquer lixo acumulado no buffer
# --- 4. Sincronização Inicial ---
# Lê byte a byte até encontrar o primeiro START_FLAG
while True:
byte_lido = ser.read(1)
if not byte_lido:
print("Timeout! Arduino não enviou dados para sincronia
inicial.")
raise serial.SerialTimeoutException()
if byte_lido == START_FLAG:
print("Sincronizado (Start Flag encontrado)! Coletando
dados...")
break
# --- 5. Loop de Coleta (N-Total) ---
# Neste ponto, acabamos de ler um START_FLAG.
# O próximo byte no buffer serial DEVE ser o primeiro byte de
DADOS.
for i in range(N_TOTAL):
# 1. Lê os bytes de DADOS (4 bytes)
data_bytes = ser.read(DATA_SIZE)
# 2. Lê o END_FLAG (1 byte)
footer_byte = ser.read(1)
# Verifica se leu a quantidade correta de bytes
if len(data_bytes) == DATA_SIZE and len(footer_byte) == 1:
# --- VALIDAÇÃO DO PACOTE ---
if footer_byte == END_FLAG:
# Pacote Válido! Decodifica e salva.
out_val, = struct.unpack(DATA_FORMAT, data_bytes)
current_time = i * dt
csv_writer.writerow([current_time, out_val])
# --- PREPARAÇÃO PARA A PRÓXIMA AMOSTRA ---
# O próximo byte OBRIGATORIAMENTE deve ser um novo
START_FLAG.
# Lemos ele agora para confirmar a sincronia contínua.
if i < N_TOTAL - 1: # Não precisamos ler o start
depois da última amostra
next_header = ser.read(1)
if next_header != START_FLAG:
print(f"Perda de Sincronia na amostra {i}!
(Esperava AA, veio {next_header.hex()})")
# Entra no modo de re-sincronização
while True:
possible_start = ser.read(1)
if not possible_start: break # Timeout
if possible_start == START_FLAG:
# Re-sincronizado
break
else:
# O footer não era 0x55. Provavelmente lemos lixo ou
deslocamos os bytes.
print(f"Erro de Footer na amostra {i}! (Veio
{footer_byte.hex()} em vez de 55)")
# Re-sincroniza
while ser.read(1) != START_FLAG:
pass
else:
print(f"Timeout de leitura na amostra {i}.")
break
print(f"\nColeta de {N_TOTAL} amostras concluída.")
except serial.SerialException as e:
print(f"\nERRO SERIAL: {e}")
except serial.SerialTimeoutException:
print("Timeout de comunicação.")
except KeyboardInterrupt:
print("\nColeta interrompida pelo usuário.")
finally:
# --- 6. Limpeza ---
if ser and ser.is_open:
ser.close()
print(f"Porta {port} fechada.")
if csv_file:
csv_file.close()
print(f"Arquivo {filename} salvo.")
# --- Executa a função principal ---
if __name__ == "__main__":
iniciar_coleta_N()
