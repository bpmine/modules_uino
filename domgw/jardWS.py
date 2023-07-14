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


@app.route('/rempli/<name>', methods=['GET'])
def rempli_set_on(name):
    global r
    
    if check_wiio_module_name(name)==False:
        abort(404)

    rr=None
    for r in remplissages:
        if r.name==name:
            rr=r
            break

    if rr==None:
        return "Pas de remplissage trouve"
    
    on=request.args.get('on',default=None)    
    if on!=None:        
        if on=='1':
            rr.on=True
        else: 
            rr.stop()

        print('Change le ON de %s en %s' % (name,rr.on) )
        
    limit_src=request.args.get('limit_src',default=None)    
    if limit_src!=None:        
        rr.limit_src=int(limit_src)
        print('Change la limite de la source %s en %s' % (name,rr.limit_src) )

    cons_tgt=request.args.get('cons_tgt',default=None)    
    if cons_tgt!=None:        
        rr.cons_tgt=int(cons_tgt)
        print('Change la consigne de la destination %s en %s' % (name,rr.cons_tgt) )
        

    return "OK"


@app.route('/wiio', methods=['GET'])
def get_wiio():
    ret=wiioCln.getJson()
    return json.dumps(ret)

if __name__=='__main__':
    app.debug = True
    app.run(host='0.0.0.0',port=5000,debug=False)
