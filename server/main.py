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

module_handle = os.getcwd()
detector = hub.load(module_handle).signatures['default']

def countOfScores(scores):
    res=0
    for i in range(len(scores)):
        if scores[i]>thresh:
            ++res
    return res

def process(picture, pictureCode, socket):
    try:
        img = tf.io.decode_jpeg(picture.data())
        converted_img  = tf.image.convert_image_dtype(img, tf.float32)[tf.newaxis, ...]
        result = detector(converted_img)
        result = {key:value.numpy() for key,value in result.items()}
        scores = result["detection_scores"]

        out = QtCore.QDataStream(socket)
    
        out << pictureCode
        out << countOfScores(scores)
        for i in range(len(scores)):
            if scores[i]>thresh:
                out << QtCore.QByteArray(bytearray(result["detection_boxes"][i]))
                out << QtCore.QByteArray(bytearray(result["detection_class_entities"][i]))
                out << QtCore.QByteArray(bytearray(result["detection_scores"][i]))
    except Exception:
        print(pictureCode, ': wrong result of processing')
    return

class MySocket:
    socket = QtNetwork.QTcpSocket()
    def socket_disconnect(self):
        self.socket.close()
        for s in socketList:
            if (s.socket.state()==QAbstractSocket.UnconnectedState or s.socket.state() == QAbstractSocket.ClosingState):
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
        process(picture, pictureCode, self.socket)
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
    
    sys.exit(app.exec())
