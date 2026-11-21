
%ver freq que compõe o sinal

close all

%tamanhoVetor = 2^nextpow2(70000);
%entrada_correta = t0001ch1 * (-1)
tamanhoVetor = 12500;
sinal_cortado = comcarga(1:tamanhoVetor, 2);
plot(comcarga(1:tamanhoVetor, 1), sinal_cortado)

janela = hanning(tamanhoVetor);
 
sinal_janelado = sinal_cortado .*janela;

fftsinalpuro = fft(sinal_janelado);

fsa=31250;%freq. amostragem

fz=(linspace(0, fsa, tamanhoVetor));

figure()
plot(fz, abs((fftsinalpuro)/tamanhoVetor));

%fazendo o filtro
sinal_filtrado = filter(Hdt, comcarga(1:end,2));

figure()
plot(comcarga(1:tamanhoVetor, 1), sinal_filtrado(1:tamanhoVetor))
    
%fft_sinal_filtrado=fft(sinal_filtrado(1:tamanhoVetor,2));
fft_sinal_filtrado = fft(sinal_filtrado(1:tamanhoVetor));
figure()
plot(fz, abs((fft_sinal_filtrado)/tamanhoVetor))

