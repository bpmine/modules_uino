#!/usr/bin/python3
from flask import Flask,request
import json
import time
from jard_wemos_mqtt_client import RdWiioClient

wiioCln = RdWiioClient('192.168.3.200')

app = Flask(__name__)

@app.route('/test', methods=['GET'])
def test():
    return 'ok'


@app.route('/wiio/modules/<name>', methods=['GET'])
def get_wiio_info_by_name(name):
    if name==None:
        abort(404)

    if wiioCln.hasModule(name)==False:
        abort(404)
        
    return json.dumps(wiioCln.getModuleJson(name));


@app.route('/wiio/modules', methods=['GET'])
def get_wiio_modules():
    modules=[]
    if 'modules' in wiioCln.getJson():
        for m in wiioCln.getJson()['modules']:
            modules.append(m)

    return json.dumps(modules)


@app.route('/states', methods=['GET'])
def get_states():
    ret={'modules':[]}
    js=wiioCln.getJson()
    if 'modules' in js:
        for k,m in js['modules'].items():
            ret['modules'].append(m)
    
    return json.dumps(ret)


@app.route('/wiio', methods=['GET'])
def get_wiio():
    ret=wiioCln.getJson()
    return json.dumps(ret)

@app.route('/wiio/do/on', methods=['GET'])
def wiio_on():
    wiioCln.setOn(True);
    return {'result':True,'msg':'MODULE WIIO ON.'}

@app.route('/wiio/do/off', methods=['GET'])
def wiio_off():
    wiioCln.setOn(False);
    return {'result':True,'msg':'MODULE WIIO OFF.'}

@app.route('/wiio/modules/<name>/do/test', methods=['GET'])
def wiio_pump_test(name):
    if wiioCln.hasModule(name)==False:
        abort(404)

    wiioCln.setCmd(name,True,10)
    return {'result':True,'msg':'Test envoyé avec succès.'}

@app.route('/wiio/modules/<name>/do/on', methods=['GET'])
def wiio_pump_on(name):
    if wiioCln.hasModule(name)==False:
        abort(404)

    duree=request.args.get('duration')
    if duree==None or not duree.isnumeric() or int(duree)>3600:
        duree=15*60
    else:
        duree=int(duree,10)
        
    wiioCln.setCmd(name,True,duree)
    
    return {'result':True,'msg':'Pompe %s ON.' % name}

@app.route('/wiio/modules/<name>/do/off', methods=['GET'])
def wiio_pump_off(name):
    if wiioCln.hasModule(name)==False:
        abort(404)

    wiioCln.setCmd(name,False,None)
    return {'result':True,'msg':'Pompe %s OFF.' % name}

if __name__=='__main__':
    app.debug = True
    app.run(host='0.0.0.0',port=5000,debug=False)

