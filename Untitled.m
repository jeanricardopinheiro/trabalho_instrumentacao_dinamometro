
% Análise espectral do sinal original e filtrado

close all
sinal = saida;

% Parâmetros
N = 125000;
fs = 312500;

% Recorte do sinal
sinal_cortado = sinal(1:N, 2);
tempo = sinal(1:N, 1);

% Janela de Hann
janela = hann(N);
Gc = sum(janela)/N;

% ==============================
% SINAL ORIGINAL NO TEMPO
% ==============================

figure()
plot(tempo, sinal_cortado)
xlabel('Tempo (s)')
ylabel('Amplitude (V)')
title('Sinal original no domínio do tempo')
grid on

% ==============================
% FFT DO SINAL ORIGINAL
% ==============================

sinal_janelado = sinal_cortado .* janela;
X = fft(sinal_janelado);

P1 = abs(X)/(N*Gc);
P1 = P1(1:floor(N/2)+1);

% Correção do espectro unilateral
P1(2:end-1) = 2*P1(2:end-1);

% Eixo de frequências
f = fs*(0:floor(N/2))/N;

figure()
plot(f, P1)
xlabel('Frequência (Hz)')
ylabel('Amplitude (V)')
title('FFT do sinal original')
grid on

% ==============================
% FILTRAGEM
% ==============================

sinal_filtrado = filter(Hdt, sinal_cortado);

figure()
plot(tempo, sinal_filtrado)
xlabel('Tempo (s)')
ylabel('Amplitude (V)')
title('Sinal filtrado no domínio do tempo')
grid on

% ==============================
% FFT DO SINAL FILTRADO
% ==============================

sinal_filtrado_janelado = sinal_filtrado .* janela;
X_filtrado = fft(sinal_filtrado_janelado);

P1_filtrado = abs(X_filtrado)/(N*Gc);
P1_filtrado = P1_filtrado(1:floor(N/2)+1);

% Correção do espectro unilateral
P1_filtrado(2:end-1) = 2*P1_filtrado(2:end-1);

figure()
plot(f, P1_filtrado)
xlabel('Frequência (Hz)')
ylabel('Amplitude (V)')
title('FFT do sinal filtrado')
grid on
