import sys
from PySide6 import QtCore
from PySide6.QtWidgets import QApplication
from PySide6 import QtNetwork

import tensorflow as tf
import tensorflow_hub as hub
import os

thresh = 0.5

server = QtNetwork.QTcpServer()
socketList = []

def initNet():
    module_handle = os.getcwd()
    detector = hub.load(module_handle).signatures['default']
    return

def process(picture, socket):
    img = tf.image.decode_jpeg(data, channels=3)
    converted_img  = tf.image.convert_image_dtype(img, tf.float32)[tf.newaxis, ...]
    result = detector(converted_img)
    result = {key:value.numpy() for key,value in result.items()}
    scores = result["detection_scores"]
    
    return

class MySocket:
    socket = QtNetwork.QTcpSocket()
    def socket_disconnect(self):
        self.socket.close()
        for s in socketList:
            if (s.state()==QAbstractSocket.UnconnectedState or s.state() == QAbstractSocket.ClosingState):
                socketList.remove(s)
        return
    def readAndProcess(self):
        into = QtCore.QDataStream(self.socket)
        into.startTransaction()
        pictureCode = QtCore.QByteArray()
        into>>pictureCode
        picture = QtCore.QByteArray()
        into>>picture
        if (not into.commitTransaction()):
            return
        process(picture, self.socket)
        return

def server_newConnection():
    print('newConnection')
    socket=MySocket()
    socket.socket = server.nextPendingConnection()
    socketList.append(socket)
    socket.socket.disconnected.connect(socket.socket_disconnect)
    socket.socket.readyRead.connect(socket.readAndProcess)
    return

if __name__ == "__main__":
    app = QApplication([])
    server.listen(port=55555)
    print(server.serverPort())
    server.newConnection.connect(server_newConnection)

    initNet()
    
    sys.exit(app.exec())
