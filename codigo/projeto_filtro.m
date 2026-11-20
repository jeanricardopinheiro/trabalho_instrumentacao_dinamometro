% Created by Tom Lankhorst (https://tomlankhorst.nl/filter-controller-cpp-implementation/)

% Adapted by Leandro Chies in October 2023 --> V0
% Novembro 2025 --> V1 (Descrição do código)
% ===================================================================
% Script para projetar o filtro IIR, criar o objeto 'Hd' no
% Workspace, e então extrair os coeficientes para o C.
% ===================================================================
clc;
close all;
% --- 1. Definição e Projeto do Filtro (Cria o objeto 'Hd') ---
% Usar o filterDesigner ou fdatool na linha de comando
% e exportar 'Hd' para o Workspace manualmente (Passo 3).

% Frequência de Amostragem (Hz)
% IMPORTANTE: Este valor DEVE ser idêntico ao Fs REAL
% calculado no "Passo 1" do tutorial

% --- 2. Extração dos Coeficientes do Objeto 'Hd' ---
% Este script lê o objeto 'Hd' que deve estar no Workspace
% Verifica se 'Hd' existe antes de tentar acessá-lo
if ~exist('Hd', 'var')
error('O objeto de filtro "Hd" não foi encontrado no Workspace.');
end
% Extrai as propriedades do objeto 'Hd'
scaleValues = Hd.ScaleValues; % Vetor de ganhos/escalas
sosMatrix = Hd.sosMatrix; % Matriz de coeficientes [b0 b1 b2 a0 a1 a2]
numSections = size(sosMatrix, 1);

i = 0;
% Itera pela matriz SOS (transposta)
% 's' se torna uma coluna [b0; b1; b2; a0; a1; a2]
for s = sosMatrix.'
i = i + 1;
% Imprime a linha de código C formatada
% A função C 'BiQuad_initWithCoefficients' espera:
% (ponteiro, b0, b1, b2, a1, a2, escala)
%
% *** OBSERVAÇÃO IMPORTANTE ***
% Note que pulamos s(4), que é o coeficiente 'a0'.
% Isso ocorre porque o MATLAB sempre normaliza a matriz SOS
% (Seções de Segunda Ordem) para que a0 seja 1.0.
% A implementação C (Forma Direta II Transposta) foi
% projetada especificamente para esta forma normalizada (a0=1),
% o que economiza uma operação de divisão e melhora a eficiência.
fprintf('BiQuad_initWithCoefficients(prtbq%d, %.8e, %.8e, %.8e, %.8e,%.8e, %.12e );\n',...
        i, s(1), s(2), s(3), s(5), s(6), scaleValues(i));
end
fprintf('\n//=============================================================\n');
