using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using jardcmd.core;

namespace jardcmd.ctrl
{
    public partial class CtrlPmp : UserControl
    {
        Control []panels= { }; 
        private string name;

        public delegate void OnClickPumpFunc(string name);
        public OnClickPumpFunc OnClickPump;

        public CtrlPmp()
        {
            InitializeComponent();
            panels=new Control[] {panel1,panel2,panel3 };
        }

        private void panelBottom_Paint(object sender, PaintEventArgs e)
        {

        }

        private void SetLevel(Label lbl,bool lvl)
        {
            if (lvl==false)
            {
                lbl.BackColor=SystemColors.Control;
                lbl.ForeColor=Color.Black;
            }
            else 
            { 
                lbl.BackColor=Color.Blue;
                lbl.ForeColor=Color.White;
            }
        }

        public void UpdatePump(WifiIO io)
        {
            if (io==null)
            { 
                name=null;
                lblName.Text="";
                SetLevel(lblBas,false);
                SetLevel(lblMoy,false);
                SetLevel(lblHaut,false);

                lblRSSI.Text="";
                lblBatt.Text="";
                btnPump.BackColor=SystemColors.Control;
                btnPump.ForeColor=Color.Black;
                panel1.BackColor=SystemColors.Info;
            
                foreach (Control c in panels)
                { 
                    c.BackColor=SystemColors.Info;
                    c.ForeColor=Color.Black;
                }
                return;
            }

            name=io.Name;
            lblName.Text=io.Name;
            SetLevel(lblBas,io.N1);
            SetLevel(lblMoy,io.N2);
            SetLevel(lblHaut,io.N3);

            lblRSSI.Text="RSSI= "+(0xFFFF-io.RSSI).ToString();
            lblBatt.Text="Batt= "+io.PWR.ToString();

            if (io.Cmd==true)            
            {
                btnPump.BackColor=Color.Green;
                btnPump.ForeColor=Color.White;
            }
            else
            {
                btnPump.BackColor=SystemColors.Control;
                btnPump.ForeColor=Color.Black;
            }            
            
            foreach (Control c in panels)
                if (io.Valid==false)
                {
                    c.BackColor=Color.Red;
                    c.ForeColor=Color.White;
                }
                else 
                {
                    c.BackColor=SystemColors.Info;
                    c.ForeColor=Color.Black;
                }
        }

        private void btnPump_Click(object sender, EventArgs e)
        {
            if (OnClickPump!=null)
                OnClickPump(name);
        }
    }
}
