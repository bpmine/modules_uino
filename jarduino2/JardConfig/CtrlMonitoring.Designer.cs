
namespace JardConfig
{
    partial class CtrlMonitoring
    {
        /// <summary> 
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur de composants

        /// <summary> 
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas 
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            this.grpMonitor = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.lblBattVoltage = new System.Windows.Forms.Label();
            this.lblSunVoltage = new System.Windows.Forms.Label();
            this.lblHum = new System.Windows.Forms.Label();
            this.lblTemp = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.lblTmr2 = new System.Windows.Forms.Label();
            this.lblTmr1 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.lblPmp1 = new System.Windows.Forms.Label();
            this.lblPmp2 = new System.Windows.Forms.Label();
            this.lblVeille = new System.Windows.Forms.Label();
            this.lblBatt = new System.Windows.Forms.Label();
            this.lblSun = new System.Windows.Forms.Label();
            this.grpMonitor.SuspendLayout();
            this.SuspendLayout();
            // 
            // grpMonitor
            // 
            this.grpMonitor.Controls.Add(this.lblSun);
            this.grpMonitor.Controls.Add(this.lblBatt);
            this.grpMonitor.Controls.Add(this.lblVeille);
            this.grpMonitor.Controls.Add(this.lblPmp2);
            this.grpMonitor.Controls.Add(this.lblPmp1);
            this.grpMonitor.Controls.Add(this.lblTmr2);
            this.grpMonitor.Controls.Add(this.lblTmr1);
            this.grpMonitor.Controls.Add(this.label7);
            this.grpMonitor.Controls.Add(this.label8);
            this.grpMonitor.Controls.Add(this.lblHum);
            this.grpMonitor.Controls.Add(this.lblTemp);
            this.grpMonitor.Controls.Add(this.label5);
            this.grpMonitor.Controls.Add(this.label6);
            this.grpMonitor.Controls.Add(this.lblSunVoltage);
            this.grpMonitor.Controls.Add(this.lblBattVoltage);
            this.grpMonitor.Controls.Add(this.label2);
            this.grpMonitor.Controls.Add(this.label1);
            this.grpMonitor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.grpMonitor.Location = new System.Drawing.Point(0, 0);
            this.grpMonitor.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.grpMonitor.Name = "grpMonitor";
            this.grpMonitor.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.grpMonitor.Size = new System.Drawing.Size(498, 93);
            this.grpMonitor.TabIndex = 0;
            this.grpMonitor.TabStop = false;
            this.grpMonitor.Text = "Surveillance";
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(5, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(58, 23);
            this.label1.TabIndex = 0;
            this.label1.Text = "Batterie";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(5, 38);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(58, 23);
            this.label2.TabIndex = 1;
            this.label2.Text = "Panneau";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lblBattVoltage
            // 
            this.lblBattVoltage.Location = new System.Drawing.Point(69, 15);
            this.lblBattVoltage.Name = "lblBattVoltage";
            this.lblBattVoltage.Size = new System.Drawing.Size(48, 23);
            this.lblBattVoltage.TabIndex = 2;
            this.lblBattVoltage.Text = "12 V";
            this.lblBattVoltage.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lblSunVoltage
            // 
            this.lblSunVoltage.Location = new System.Drawing.Point(72, 38);
            this.lblSunVoltage.Name = "lblSunVoltage";
            this.lblSunVoltage.Size = new System.Drawing.Size(45, 23);
            this.lblSunVoltage.TabIndex = 3;
            this.lblSunVoltage.Text = "12 V";
            this.lblSunVoltage.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lblHum
            // 
            this.lblHum.Location = new System.Drawing.Point(198, 38);
            this.lblHum.Name = "lblHum";
            this.lblHum.Size = new System.Drawing.Size(37, 23);
            this.lblHum.TabIndex = 7;
            this.lblHum.Text = "14%";
            this.lblHum.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lblTemp
            // 
            this.lblTemp.Location = new System.Drawing.Point(198, 15);
            this.lblTemp.Name = "lblTemp";
            this.lblTemp.Size = new System.Drawing.Size(37, 23);
            this.lblTemp.TabIndex = 6;
            this.lblTemp.Text = "40°C";
            this.lblTemp.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label5
            // 
            this.label5.Location = new System.Drawing.Point(123, 38);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(69, 23);
            this.label5.TabIndex = 5;
            this.label5.Text = "Humidité";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label6
            // 
            this.label6.Location = new System.Drawing.Point(123, 15);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(69, 23);
            this.label6.TabIndex = 4;
            this.label6.Text = "Température";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lblTmr2
            // 
            this.lblTmr2.Location = new System.Drawing.Point(316, 38);
            this.lblTmr2.Name = "lblTmr2";
            this.lblTmr2.Size = new System.Drawing.Size(50, 23);
            this.lblTmr2.TabIndex = 11;
            this.lblTmr2.Text = "0 min";
            this.lblTmr2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lblTmr1
            // 
            this.lblTmr1.Location = new System.Drawing.Point(316, 15);
            this.lblTmr1.Name = "lblTmr1";
            this.lblTmr1.Size = new System.Drawing.Size(50, 23);
            this.lblTmr1.TabIndex = 10;
            this.lblTmr1.Text = "50 min";
            this.lblTmr1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label7
            // 
            this.label7.Location = new System.Drawing.Point(241, 38);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(69, 23);
            this.label7.TabIndex = 9;
            this.label7.Text = "Tmr Pmp 2";
            this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label8
            // 
            this.label8.Location = new System.Drawing.Point(241, 15);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(69, 23);
            this.label8.TabIndex = 8;
            this.label8.Text = "Tmr Pmp 1";
            this.label8.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // lblPmp1
            // 
            this.lblPmp1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblPmp1.Location = new System.Drawing.Point(22, 61);
            this.lblPmp1.Name = "lblPmp1";
            this.lblPmp1.Size = new System.Drawing.Size(52, 23);
            this.lblPmp1.TabIndex = 12;
            this.lblPmp1.Text = "PMP 1";
            this.lblPmp1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblPmp2
            // 
            this.lblPmp2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblPmp2.Location = new System.Drawing.Point(83, 61);
            this.lblPmp2.Name = "lblPmp2";
            this.lblPmp2.Size = new System.Drawing.Size(52, 23);
            this.lblPmp2.TabIndex = 13;
            this.lblPmp2.Text = "PMP 2";
            this.lblPmp2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblVeille
            // 
            this.lblVeille.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblVeille.Location = new System.Drawing.Point(141, 61);
            this.lblVeille.Name = "lblVeille";
            this.lblVeille.Size = new System.Drawing.Size(52, 23);
            this.lblVeille.TabIndex = 14;
            this.lblVeille.Text = "Veille";
            this.lblVeille.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblBatt
            // 
            this.lblBatt.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblBatt.Location = new System.Drawing.Point(201, 61);
            this.lblBatt.Name = "lblBatt";
            this.lblBatt.Size = new System.Drawing.Size(52, 23);
            this.lblBatt.TabIndex = 15;
            this.lblBatt.Text = "BATT";
            this.lblBatt.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lblSun
            // 
            this.lblSun.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.lblSun.Location = new System.Drawing.Point(259, 61);
            this.lblSun.Name = "lblSun";
            this.lblSun.Size = new System.Drawing.Size(52, 23);
            this.lblSun.TabIndex = 16;
            this.lblSun.Text = "SUN";
            this.lblSun.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // CtrlMonitoring
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.grpMonitor);
            this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.Name = "CtrlMonitoring";
            this.Size = new System.Drawing.Size(498, 93);
            this.grpMonitor.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox grpMonitor;
        private System.Windows.Forms.Label lblHum;
        private System.Windows.Forms.Label lblTemp;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label lblSunVoltage;
        private System.Windows.Forms.Label lblBattVoltage;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lblTmr2;
        private System.Windows.Forms.Label lblTmr1;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label lblSun;
        private System.Windows.Forms.Label lblBatt;
        private System.Windows.Forms.Label lblVeille;
        private System.Windows.Forms.Label lblPmp2;
        private System.Windows.Forms.Label lblPmp1;
    }
}
