echo 'nitro' >> __client_0_pipe__
echo 'nitro' >> __client_1_pipe__
sleep 1
# El siguiente nitro del cliente 0 no deberia funcionar, porque paso solo un segundo.
echo 'nitro' >> __client_0_pipe__
echo 'read 2' >> __client_0_pipe__
echo 'read 2' >> __client_1_pipe__
sleep 1
# El siguiente nitro del cliente 0 no deberia funcionar, porque pasaron solo dos segundos.
echo 'nitro' >> __client_0_pipe__
echo 'nitro' >> __client_2_pipe__
echo 'read 3' >> __client_2_pipe__
sleep 1
# En este intervalo terminan los nitro de 0 y 1.
sleep 1
# Este nitro del cliente 0 sí se efectúa porque ya pasaron 4 segundos.
echo 'nitro' >> __client_0_pipe__




echo 'read 6' >> __client_0_pipe__
echo 'read 6' >> __client_1_pipe__
echo 'read 3' >> __client_2_pipe__
echo 'read 2' >> __client_2_pipe__

# Agrego el sleep para asegurarme que el server envia las
# acciones de nitro finalizado.
sleep 4

echo 'exit' >> __client_0_pipe__
echo 'exit' >> __client_1_pipe__
echo 'exit' >> __client_2_pipe__
