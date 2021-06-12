import pika
import requests
import json
import demjson
import sqlite3
from pymongo import MongoClient

class ReadJardin:
    ip=None
    login=None
    passe=None

    conn=None
    ch=None

    def connect(self):
        credentials = pika.PlainCredentials(self.login, self.passe)

        self.conn = pika.BlockingConnection(
            pika.ConnectionParameters(self.ip,
                                      5672,
                                      '/',
                                      credentials)
            )
        self.ch = self.conn.channel()
        
    
    def __init__(self,ip,login,passe):
        self.ip=ip
        self.login=login
        self.passe=passe

        self.connect()

    def send(self,msg):
        self.ch.basic_publish(exchange="amq.topic",
                              routing_key=".jarduino.cmd",
                              body=msg)

    def close(self):
        self.ch.close()
        self.conn.close()

    def intern_cb(self,ch, method, properties, body):
        key=method.routing_key
        msg=body.decode()
        if (self.callback!=None):
            self.callback(key,msg);

        #self.ch.basic_ack(delivery_tag=method.delivery_tag)
        
    def start(self,callback):
        self.callback=callback;
        self.ch.basic_consume(queue="jarduino", on_message_callback=self.intern_cb)

        self.ch.start_consuming()

IP_TOUR='192.168.1.42'





with open('credentials.txt','r') as json_file:
    data = json.load(json_file)

IP=data['ip']
LOGIN=data['login']
PASS=data['pass']

r=ReadJardin(IP,LOGIN,PASS)


mg=MongoClient("mongodb://127.0.0.1:27017");
db=mg.jarduino;
logs = db["logs"]

def cb(key,msg):
    print("Message recu: %s: %s" % (key,msg))
    if (key==".jarduino.log"):
        obj=demjson.decode(msg)
        print(obj)
        logs.insert_one(obj)


r.start(cb)

        



