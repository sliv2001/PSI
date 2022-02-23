import sys
from PySide6 import QtCore
from PySide6.QtWidgets import QApplication
from PySide6 import QtNetwork

server = QtNetwork.QTcpServer()
socketList = []

def socket_disconnect():
    print('socket disconnect')
    return

def readAndProcess():
    print('readAndProcess')
    return

def server_newConnection():
    print('newConnection')
    socket=server.nextPendingConnection()
    socketList.append(socket)
    socket.disconnected.connect(socket_disconnect)
    socket.readyRead.connect(readAndProcess)
    return

if __name__ == "__main__":
    app = QApplication([])
    server.listen(port=55555)
    server.newConnection.connect(server_newConnection)
    sys.exit(app.exec())
