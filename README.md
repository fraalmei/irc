# IRC Server

Este proyecto consiste en la implementación de un servidor IRC (Internet Relay Chat) en C++98, utilizando sockets y el protocolo TCP/IP. Para la evaluación, se utiliza un cliente de IRC de uso extendido, como el HexChat que está actualmente instalado en los Mac de los clusters de 42.

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

## ⌨️ Uso de las funcionalidades de nuestro servidor
   En el lado del cliente, los comandos habitualmente se envían con `/`, mientras que el servidor los recibirá 'traducidos' por parte del cliente. Por ejemplo, escribiendo `/join #mychannel` en el cliente, el servidor recibirá el mensaje `user42: JOIN #mychannel :`. Al escribir un mensaje en una ventana de canal al que ya esté unido, el usuario puede escribir directamente `Hola a todo el mundo`, mientras que el servidor recibirá en ese caso `user42: PRVTMSG #mychannel :Hola a todo el mundo` y tratará el mensaje para enviarlo a todos los usuarios presentes en ese canal.

1. **Conexión:**
   El software cliente debe intentar una conexión a través de TCP/IP a la IP donde esté corriendo el servidor a través del puerto que esté abierto para ese servicio. Tras detectar la nueva conexión, el servidor esperará desde el cliente los siguientes mensajes en cualquier orden:

   NICK <name>\r\n
   USER <name>\r\n
   PASS <password>\r\n
   CAP <capabilities> (opcional)

   Nuestro server podrá reconocer el mensaje de CAP para enviar su propio mensaje CAP básico,  no forma parte de este proyecto realizar la negociación de capacidades.

   Si el cliente envía correctamente los tres mensajes necesarios y el password coincide con el del servidor, el usuario será reconocido y podrá enviar comandos al servidor.

2. **Entrar y salir de canales:**
   El resto de comandos requeridos para este proyecto están relacionados con canales y el comportamiento entre los usuarios y éstos. Los dos primeros son para cualquier usuario que quiera entrar o salir de un canal.

   El comando JOIN sirve para entrar a un canal, si ese canal no existe, se crea uno nuevo con ese nombre y el usuario que lo ha creado recibe atributo de operador para ese canal. La entrada al canal puede estar restringida por los modos del canal (ver más abajo).
   
   El comando PART sirve para abandonar un canal.

3. **Afectar canales:**
   El resto de los comandos son para operadores para tener un control sobre los usuarios en ese canal.

   El comando KICK sirve para expulsar a un usuario del canal. Al no haber un comando BAN implementado, el usuario puede volver a entrar con un nuevo JOIN.

   El comando INVITE envía una notificación de invitación de parte de un operador que ya esté en un canal para entrar en ese canal. Puede ser necesaria esta invitación si el modo del canal ha cambiado a invite-only.

   El comando TOPIC cambia el mensaje automático que se envía a quien entra al canal.

   El comando MODE cambia atributos del canal o de un usuario sobre ese canal. Tras el nombre del canal hay que añadir un flag con prefijo `+` o `-`:
      +i / -i añade o elimina la restricción de haber recibido una invitación para poder entrar al canal.
      +t / -t protege o permite cambios al usar el comando TOPIC para los propios operadores.
      +k / -k añade o elimina la necesidad de usar un password para entrar al canal. Debe ir seguido del password como parámetro en caso de ser `+k`.
      +o / -o otorga o retira la capacidad de operador. Debe ir seguido del usuario a quien se le otorga o retira el permiso.
      +l / -l añade o elimina un límite de usuarios en el canal, no expulsa usuarios si ese nuevo límite ya se había sobrepasado, pero impide que entren nuevos si la cantidad actual de usuarios en el canal no es inferior al límite. Debe ir seguido de la cifra límite como parámetro en caso de ser `+l`.