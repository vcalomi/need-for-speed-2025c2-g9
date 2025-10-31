echo 'nitro' >> __client_0_pipe__
echo 'nitro' >> __client_0_pipe__
echo 'nitro' >> __client_0_pipe__
echo 'nitro' >> __client_0_pipe__
sleep 1
echo 'nitro' >> __client_0_pipe__
echo 'nitro' >> __client_0_pipe__
echo 'nitro' >> __client_0_pipe__
echo 'nitro' >> __client_0_pipe__

# Solo debería procesar el primer nitro, los demas son ignorados por el server.

echo 'read 2' >> __client_0_pipe__

echo 'nitro' >> __client_0_pipe__

sleep 7
# Agrego el sleep para asegurarme que el server envia el
# mensaje de nitro activado y luego desactivado dos veces.

echo 'exit' >> __client_0_pipe__

