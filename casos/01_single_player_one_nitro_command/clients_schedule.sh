echo 'nitro' >> __client_0_pipe__
# Agrego el sleep para asegurarme que el server envia el
# mensaje de nitro activado y luego desactivado.

sleep 4

echo 'exit' >> __client_0_pipe__
