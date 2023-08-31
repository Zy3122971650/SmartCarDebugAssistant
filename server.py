datas = datas2 = bytearray(
[0xAA,0x00,0x03,0x01,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x3F,0x00,0x00,0x00,0x40,0x00,0x00,0x40,0x40,0x00,0x00,0x80,0x40,0x00,0x00,0xA0,0x40,0x00,0x00,0xC0,0x40,0x00,0x00,0xE0,0x40,0x00,0x00,0x00,0x41,])
import socket

# 创建一个套接字对象
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 绑定服务器地址和端口
server_address = ('localhost', 2419)
server_socket.bind(server_address)

# 开始监听连接
server_socket.listen(5)

print("服务器已启动，等待客户端连接...")
count = 0
while True:
    # 等待客户端连接
    client_socket, client_address = server_socket.accept()
    print("客户端已连接：", client_address)
    while 1:
        input()
        if count %2==0:
            data = datas
        else:
            data = datas2
        
        data[1] = count
        client_socket.send(data)
        count+=1
        if count >255:
            count=0