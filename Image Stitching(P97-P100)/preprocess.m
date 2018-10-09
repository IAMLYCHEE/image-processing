function data = preprocess(data)
indice = find(data>40);
data(indice) = min(data(indice)+30,255);