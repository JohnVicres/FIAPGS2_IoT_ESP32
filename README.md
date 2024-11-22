# GS2 - Resfriamento líquido e sustentabilidade

### Vídeo 
[![The future of liquid cooling for data centers](https://img.youtube.com/vi/DS-UmJWFPqU/0.jpg)](https://www.youtube.com/watch?v=DS-UmJWFPqU "The future of liquid cooling for data centers")

## Propósito
A ideia motivadora deste projeto está em disseminar a compreensão da melhor eficiência energética de sistemas de resfriamento líquido para servidores[^1], comparada à eficiência do resfriamento aéreo.

Embora haja muita evidência da utilidade do resfriamento líquido, há ressalvas em sua implementação, geralmente sobre o custo de adaptação (de um sistema de refrigeração para outro) e o custo direto da aparelhagem. Há, porém, um horizonte de diminuição de custo respectivo ao segundo caso: a reutilização de óleo vegetal "usado", como líquido refrigerante (ao invés do ubíquito óleo mineral)[^2]. E isso não só abateria parte do custo, como seria uma excelente melhoria na sustentabilidade ecológica ligada à manutenção de servidores, afinal seria possível a reciclagem de um resíduo extremamente comum (óleo de cozinha!) para substituir um derivado de petróleo.

Concluindo, é de grande importância para o mundo que as inúmeras empresas que possuem servidores estejam mais abertas às vantagens consideráveis do resfriamento líquido. Além disso, embora não caiba à minha pretendida área de trabalho, seria um avanço capital na diminuição da pegada de carbono de servidores (que carece de ser trabalhada com urgência, considerando a importância da Nuvem de dados vs. o seu consumo energético[^3]) a reciclagem, ou ao menos adaptação, do óleo vegetal aos sistemas de resfriamento líquido.

[^1]:[Diving Deep into the Liquid Server Cooling Choices](https://www.dell.com/en-us/blog/diving-deep-into-the-liquid-server-cooling-choices/)
[^2]:[Feasibility study of waste vegetable oil as an alternative cooling medium in transformers](https://www.sciencedirect.com/science/article/abs/pii/S1359431118358587)
[^3]:[The Staggering Ecological Impacts of Computation and the Cloud](https://thereader.mitpress.mit.edu/the-staggering-ecological-impacts-of-computation-and-the-cloud/)

## Descrição do projeto
Este projeto é de um monitorador de temperatura e umidade para servidores equipados com resfriamento líquido. Ele é baseado no ESP32 DEVKIT V1 - DOIT, e implementa um sensor DHT22 para medições de temperatura e umidade do ar (na sala do servidor), e um termistor NTC (sensor analógico de temperatura) para medição imersa de temperatura (do líquido refrigerante). Não foram implementados avisos para temperaturas e/ou percentual de umidade inadequados ou emergenciais num ambiente de servidores, mas esse seria o próximo passo lógico.
