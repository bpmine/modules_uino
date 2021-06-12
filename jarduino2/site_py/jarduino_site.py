from flask import Flask, render_template
from pymongo import MongoClient
import pymongo;
import datetime;
import re;

app = Flask(__name__) 

p=re.compile("^([0-9]{4})-([0-9]{2})-([0-9]{2})T([0-9]{2}):([0-9]{2}):([0-9]{2})Z$")
def getDate(strDt):
    m=p.match(strDt)
    if (m!=None):
        year=int(m.group(1))
        month=int(m.group(2))
        day=int(m.group(3))
        hour=int(m.group(4))
        minute=int(m.group(5))
        second=int(m.group(6))

        dt=datetime.datetime(year,month,day,hour,minute,second)

        return dt

    return None

def getDatas():
    jrs=[
        'dimanche',
        'lundi',
        'mardi',
        'mercredi',
        'jeudi',
        'vendredi',
        'samedi'
        ];
    
    datas={
    'state':'On/Off ?',
    'vsun':'?',
    'vbatt':'?',
    'last':'?',
    'now':'?'
    }
    
    mg=MongoClient("mongodb://127.0.0.1:27017");
    db=mg.jarduino;
    logs = db["logs"]
    res=logs.find({}).sort('date',pymongo.DESCENDING).limit(1)
    try:
        d=res[0]

        if (d['pompe']==1):
            datas['state']='ON'
        else:
            datas['state']='OFF'

        fTmp=float(int(d['vsun']))
        fTmp=fTmp/10;
        datas['vsun']='%.1f' % fTmp

        fTmp=float(int(d['vbatt']))
        fTmp=fTmp/10;
        datas['vbatt']='%.1f' % fTmp

        dte=getDate(d['date'])
        jdls=jrs[dte.weekday()]
        datas['now']="%s %02d/%02d/%02d à %02d:%02d" % (jdls,dte.day,dte.month,dte.year,dte.hour,dte.minute)

    except:
        pass;
                       
    res=logs.find({'pompe':1}).sort('date',pymongo.DESCENDING).limit(1)
    dte=getDate(res[0]['date'])
    
    print(dte)
    jdls=jrs[dte.weekday()]
    datas['last']="%s %02d/%02d/%02d à %02d:%02d" % (jdls,dte.day,dte.month,dte.year,dte.hour,dte.minute)

    mg.close()

    return datas;
    

@app.route("/") 
def home():

    datas=getDatas()    

    
    return render_template(r'index.html',datas=datas)

@app.rount("/robots.txt")
def robots():
	return "User-Agent: *\nDisallow: /"

if __name__=="__main__":    
    app.run(host="0.0.0.0",debug = False,port=80)

