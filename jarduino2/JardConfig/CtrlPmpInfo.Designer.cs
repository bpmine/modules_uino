﻿
namespace JardConfig
{
    partial class CtrlPmpInfo
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
            this.grpPmp = new System.Windows.Forms.GroupBox();
            this.numTimer = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.checkCmdRemote = new System.Windows.Forms.CheckBox();
            this.checkRemote = new System.Windows.Forms.CheckBox();
            this.checkedListDays = new System.Windows.Forms.CheckedListBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.numHour = new System.Windows.Forms.NumericUpDown();
            this.numDuration = new System.Windows.Forms.NumericUpDown();
            this.checkAuto = new System.Windows.Forms.CheckBox();
            this.checkEnabled = new System.Windows.Forms.CheckBox();
            this.checkForced = new System.Windows.Forms.CheckBox();
            this.numMin = new System.Windows.Forms.NumericUpDown();
            this.grpPmp.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numTimer)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numHour)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numDuration)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMin)).BeginInit();
            this.SuspendLayout();
            // 
            // grpPmp
            // 
            this.grpPmp.Controls.Add(this.numTimer);
            this.grpPmp.Controls.Add(this.label3);
            this.grpPmp.Controls.Add(this.button1);
            this.grpPmp.Controls.Add(this.checkCmdRemote);
            this.grpPmp.Controls.Add(this.checkRemote);
            this.grpPmp.Controls.Add(this.checkedListDays);
            this.grpPmp.Controls.Add(this.label2);
            this.grpPmp.Controls.Add(this.label1);
            this.grpPmp.Controls.Add(this.numHour);
            this.grpPmp.Controls.Add(this.numDuration);
            this.grpPmp.Controls.Add(this.checkAuto);
            this.grpPmp.Controls.Add(this.checkEnabled);
            this.grpPmp.Controls.Add(this.checkForced);
            this.grpPmp.Controls.Add(this.numMin);
            this.grpPmp.Dock = System.Windows.Forms.DockStyle.Fill;
            this.grpPmp.Location = new System.Drawing.Point(0, 0);
            this.grpPmp.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.grpPmp.Name = "grpPmp";
            this.grpPmp.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.grpPmp.Size = new System.Drawing.Size(307, 399);
            this.grpPmp.TabIndex = 0;
            this.grpPmp.TabStop = false;
            this.grpPmp.Text = "groupBox1";
            this.grpPmp.Enter += new System.EventHandler(this.groupBox1_Enter);
            // 
            // numTimer
            // 
            this.numTimer.Location = new System.Drawing.Point(160, 330);
            this.numTimer.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.numTimer.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.numTimer.Name = "numTimer";
            this.numTimer.Size = new System.Drawing.Size(63, 22);
            this.numTimer.TabIndex = 14;
            // 
            // label3
            // 
            this.label3.Location = new System.Drawing.Point(5, 330);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(137, 25);
            this.label3.TabIndex = 13;
            this.label3.Text = "Timer bouton (mins)";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(9, 362);
            this.button1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(287, 26);
            this.button1.TabIndex = 12;
            this.button1.Text = "Appliquer";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // checkCmdRemote
            // 
            this.checkCmdRemote.Location = new System.Drawing.Point(143, 86);
            this.checkCmdRemote.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.checkCmdRemote.Name = "checkCmdRemote";
            this.checkCmdRemote.Size = new System.Drawing.Size(153, 31);
            this.checkCmdRemote.TabIndex = 11;
            this.checkCmdRemote.Text = "Commande remote";
            this.checkCmdRemote.UseVisualStyleBackColor = true;
            // 
            // checkRemote
            // 
            this.checkRemote.Location = new System.Drawing.Point(5, 86);
            this.checkRemote.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.checkRemote.Name = "checkRemote";
            this.checkRemote.Size = new System.Drawing.Size(132, 31);
            this.checkRemote.TabIndex = 10;
            this.checkRemote.Text = "Remote activé";
            this.checkRemote.UseVisualStyleBackColor = true;
            // 
            // checkedListDays
            // 
            this.checkedListDays.FormattingEnabled = true;
            this.checkedListDays.Items.AddRange(new object[] {
            "Dimanche",
            "Lundi",
            "Mardi",
            "Mercredi",
            "Jeudi",
            "Vendredi",
            "Samedi"});
            this.checkedListDays.Location = new System.Drawing.Point(5, 158);
            this.checkedListDays.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.checkedListDays.Name = "checkedListDays";
            this.checkedListDays.Size = new System.Drawing.Size(132, 140);
            this.checkedListDays.TabIndex = 9;
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(148, 230);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(135, 25);
            this.label2.TabIndex = 7;
            this.label2.Text = "Durée auto (mins)";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(144, 158);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(152, 25);
            this.label1.TabIndex = 6;
            this.label1.Text = "Heure de démarrage";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // numHour
            // 
            this.numHour.Location = new System.Drawing.Point(152, 185);
            this.numHour.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.numHour.Maximum = new decimal(new int[] {
            23,
            0,
            0,
            0});
            this.numHour.Name = "numHour";
            this.numHour.Size = new System.Drawing.Size(63, 22);
            this.numHour.TabIndex = 5;
            // 
            // numDuration
            // 
            this.numDuration.Location = new System.Drawing.Point(152, 257);
            this.numDuration.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.numDuration.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.numDuration.Name = "numDuration";
            this.numDuration.Size = new System.Drawing.Size(131, 22);
            this.numDuration.TabIndex = 4;
            // 
            // checkAuto
            // 
            this.checkAuto.Location = new System.Drawing.Point(5, 122);
            this.checkAuto.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.checkAuto.Name = "checkAuto";
            this.checkAuto.Size = new System.Drawing.Size(132, 31);
            this.checkAuto.TabIndex = 3;
            this.checkAuto.Text = "Auto";
            this.checkAuto.UseVisualStyleBackColor = true;
            // 
            // checkEnabled
            // 
            this.checkEnabled.Location = new System.Drawing.Point(5, 50);
            this.checkEnabled.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.checkEnabled.Name = "checkEnabled";
            this.checkEnabled.Size = new System.Drawing.Size(155, 31);
            this.checkEnabled.TabIndex = 2;
            this.checkEnabled.Text = "Autorisée/Activée";
            this.checkEnabled.UseVisualStyleBackColor = true;
            // 
            // checkForced
            // 
            this.checkForced.Location = new System.Drawing.Point(5, 21);
            this.checkForced.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.checkForced.Name = "checkForced";
            this.checkForced.Size = new System.Drawing.Size(155, 31);
            this.checkForced.TabIndex = 1;
            this.checkForced.Text = "Forcée";
            this.checkForced.UseVisualStyleBackColor = true;
            // 
            // numMin
            // 
            this.numMin.Location = new System.Drawing.Point(220, 185);
            this.numMin.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.numMin.Maximum = new decimal(new int[] {
            59,
            0,
            0,
            0});
            this.numMin.Name = "numMin";
            this.numMin.Size = new System.Drawing.Size(63, 22);
            this.numMin.TabIndex = 0;
            // 
            // CtrlPmpInfo
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.grpPmp);
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "CtrlPmpInfo";
            this.Size = new System.Drawing.Size(307, 399);
            this.grpPmp.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.numTimer)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numHour)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numDuration)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMin)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox grpPmp;
        private System.Windows.Forms.CheckBox checkCmdRemote;
        private System.Windows.Forms.CheckBox checkRemote;
        private System.Windows.Forms.CheckedListBox checkedListDays;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown numHour;
        private System.Windows.Forms.NumericUpDown numDuration;
        private System.Windows.Forms.CheckBox checkAuto;
        private System.Windows.Forms.CheckBox checkEnabled;
        private System.Windows.Forms.CheckBox checkForced;
        private System.Windows.Forms.NumericUpDown numMin;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.NumericUpDown numTimer;
        private System.Windows.Forms.Label label3;
    }
}
