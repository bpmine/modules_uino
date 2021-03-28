import os;
import re;
import datetime;
import statistics;
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
from matplotlib.dates import DateFormatter
from matplotlib.ticker import (MultipleLocator, FormatStrFormatter,
                               AutoMinorLocator)
import matplotlib.patches as mpatches

def getFileList():
    lst=[];
    for fn in os.listdir("."):
        p=re.compile("([0-9]{2})([0-9]{2})([0-9]{2}).G");
        m=p.match(fn);
        if (m!=None):
            lst.append(fn);
            print(fn)

    return lst;


def getDate(d,h):
    p=re.compile("([0-9]{2})([0-9]{2})([0-9]{4})");
    m=p.match(d);
    if (m==None):
        p=re.compile("([0-9]{2})/([0-9]{2})/([0-9]{4})");
        m=p.match(d);
        
    if (m!=None):
        pp=re.compile("([0-9]{2}):([0-9]{2}):([0-9]{2})");
        mm=pp.match(h);
        if (mm!=None):
            return datetime.datetime(
                int(m.group(3)),
                int(m.group(2)),
                int(m.group(1)),
                int(mm.group(1)),
                int(mm.group(2)),
                int(mm.group(3))
                );

    return None;


# 24/03/2021;21:48:56;11;67;

def processFile(fn):
    datas=[];

    print("Open %s" % fn)

    fp=open(fn,"r");
    if (fp!=None):
        for ln in fp.readlines():
            elms=ln.split(";");
            if (len(elms)!=5):
                continue
            
            dte=getDate(elms[0],elms[1])
            t=int(elms[2])
            h=int(elms[3])

            elm={'date':dte,'t':t,'h':h}
            datas.append(elm)

        fp.close()
            
    return datas

def getsod(dte):
    return datetime.datetime(dte.year,dte.month,dte.day,0,0,0)


def geteod(dte):
    return datetime.datetime(dte.year,dte.month,dte.day,23,59,59)


def groupDays(datas):
    ret=[]
    start=None
    end=None
    elm={}
    
    for d in datas:
        dte=d['date']
        if (start==None):
            start=getsod(dte)
            end=geteod(dte)
            elm={}
            elm['date']=start
            elm['temps']=[]
            elm['hums']=[]

        if (dte>=start) and (dte<=end):
            elm['temps'].append(d['t'])
            elm['hums'].append(d['h'])
        else:
            ret.append(elm)
            
            elm={}
            start=getsod(dte)
            end=geteod(dte)
            elm['date']=start
            elm['temps']=[d['t']]
            elm['hums']=[d['h']]

    return ret

            
def getPlageDates(datas):
    start=datas[0]['date'];
    end=datas[-1]['date'];
    
    return "du %s au %s" % (
        start.strftime("%d/%m/%y %H:%M:%S"),
        end.strftime("%d/%m/%y %H:%M:%S")
        );        


def plot_temps(datas):
    xd=[];
    temps=[];
    hums=[]
    for d in datas:
        xd.append(d['date']);
        temps.append(d['t']);
        hums.append(d['h']);

    fig, ax1 = plt.subplots()
    ax1=plt.gca();
    
    ax1.set_title("Période %s"  %(getPlageDates(datas)));

    ax2 = ax1.twinx()

    
    ax1.plot(xd, temps,color='y')
    ax2.plot(xd, hums,color='blue')

    ax2.set_ylabel("Humidité",color='blue');
    for label in ax2.yaxis.get_ticklabels():
        label.set_color('blue')
    
    ax1.set_ylabel("Température",color='y');
    for label in ax1.yaxis.get_ticklabels():
        label.set_color('y')

    fig.gca().set_xlabel("t");
    
    
    ax1.xaxis.set_major_locator(mdates.DayLocator())
    ax1.xaxis.set_major_formatter(mdates.DateFormatter('%d/%m'))
    ax1.xaxis.set_minor_locator(mdates.HourLocator())
    ax1.xaxis.set_minor_formatter(mdates.DateFormatter('%H'))
    #fig.autofmt_xdate()
    
    red_patch = mpatches.Patch(color='blue', label="Humidité")
    obs_patch = mpatches.Patch(color='y', label="Temperature")
    plt.legend(handles=[red_patch,obs_patch])

    plt.show();

datas=[];
lst=getFileList()
for fn in lst:
    datas+=processFile(fn);

datas.sort(key=lambda d: d['date'])

days=groupDays(datas)
for d in days:
    mn=min(d['temps'])
    mean=statistics.mean(d['temps'])
    mx=max(d['temps'])
    
    mn2=min(d['hums'])
    mean2=statistics.mean(d['hums'])
    mx2=max(d['hums'])

    print("%-15s: (%4d; %4d; %4d) (%4d; %4d; %4d)" % (d['date'],mn,mean,mx,mn2,mean2,mx2))

plot_temps(datas)
