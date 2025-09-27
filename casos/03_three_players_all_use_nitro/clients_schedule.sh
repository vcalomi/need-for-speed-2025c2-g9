echo 'nitro' >> __client_0_pipe__
echo 'nitro' >> __client_0_pipe__
echo 'nitro' >> __client_0_pipe__
echo 'nitro' >> __client_0_pipe__
echo 'nitro' >> __client_0_pipe__
echo 'nitro' >> __client_0_pipe__
echo 'nitro' >> __client_0_pipe__

echo 'nitro' >> __client_1_pipe__
echo 'nitro' >> __client_1_pipe__
echo 'nitro' >> __client_2_pipe__
echo 'nitro' >> __client_2_pipe__


echo 'read 3' >> __client_0_pipe__
echo 'read 4' >> __client_1_pipe__
echo 'read 5' >> __client_2_pipe__

# Agrego el sleep para asegurarme que el server envia las
# acciones de nitro finalizado.
sleep 4

echo 'exit' >> __client_0_pipe__
echo 'exit' >> __client_1_pipe__
echo 'exit' >> __client_2_pipe__
