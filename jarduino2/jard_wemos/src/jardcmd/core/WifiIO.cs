using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace jardcmd.core
{
    public class WifiIO
    {
        protected string _Name;
        protected bool _Cmd;
        protected bool _N1;
        protected bool _N2;
        protected bool _N3;
        protected bool _Valid;
        protected int _RSSI;
        protected int _PWR;
        protected string _IP;

        public string Name { get { return _Name;}}
        public bool Cmd { get{ return _Cmd;}}
        public bool N1 { get { return _N1;}}
        public bool N2 { get{ return _N2;}}
        public bool N3 { get { return _N3;}}
        public int RSSI { get { return _RSSI;}}
        public int PWR { get { return _PWR;}}
        public bool Valid { get { return _Valid;}}

        public WifiIO()
        {
        }
    }
}
