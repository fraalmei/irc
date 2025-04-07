# IRC Server

Este proyecto consiste en la implementación de un servidor IRC (Internet Relay Chat) en C++98, utilizando sockets y el protocolo TCP/IP.

## 🧠 ¿Qué es un socket?

Un **socket** es una interfaz que permite la comunicación entre dos programas a través de una red, como Internet o una red local.  
Podés imaginarlo como un "enchufe digital" que conecta a dos programas para que puedan enviarse mensajes entre sí.

En este proyecto, usamos sockets **TCP (SOCK_STREAM)**, lo cual significa que se establece una conexión estable entre cliente y servidor antes de intercambiar datos, garantizando el orden y entrega de los mensajes.

## 🔧 ¿Cómo funciona nuestro servidor?

1. **Creación del socket:**
   El servidor crea un socket que actuará como punto de entrada para las conexiones de los clientes.

2. **Enlace a una IP y puerto:**
   Usamos `bind()` para asignar ese socket a una dirección (como `127.0.0.1`) y un puerto (`6667`, el típico de IRC).

3. **Escucha de conexiones:**
   Con `listen()`, el servidor queda "a la espera" de nuevos clientes.

4. **Acepta clientes:**
   Cada vez que un cliente se conecta, usamos `accept()` para establecer una nueva conexión dedicada a ese cliente.

5. **Comunicación:**
   A través de `recv()` y `send()`, el servidor recibe mensajes de cada cliente y les responde.

6. **Desconexión:**
   Cuando un cliente se va, el socket se cierra y el servidor sigue funcionando para los demás clientes.

