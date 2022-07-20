
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
            this.numMin = new System.Windows.Forms.NumericUpDown();
            this.checkForced = new System.Windows.Forms.CheckBox();
            this.checkEnabled = new System.Windows.Forms.CheckBox();
            this.checkAuto = new System.Windows.Forms.CheckBox();
            this.numDuration = new System.Windows.Forms.NumericUpDown();
            this.numHour = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.checkedListDays = new System.Windows.Forms.CheckedListBox();
            this.checkRemote = new System.Windows.Forms.CheckBox();
            this.checkCmdRemote = new System.Windows.Forms.CheckBox();
            this.button1 = new System.Windows.Forms.Button();
            this.grpPmp.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numMin)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numDuration)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numHour)).BeginInit();
            this.SuspendLayout();
            // 
            // grpPmp
            // 
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
            this.grpPmp.Name = "grpPmp";
            this.grpPmp.Size = new System.Drawing.Size(333, 315);
            this.grpPmp.TabIndex = 0;
            this.grpPmp.TabStop = false;
            this.grpPmp.Text = "groupBox1";
            this.grpPmp.Enter += new System.EventHandler(this.groupBox1_Enter);
            // 
            // numMin
            // 
            this.numMin.Location = new System.Drawing.Point(250, 197);
            this.numMin.Maximum = new decimal(new int[] {
            59,
            0,
            0,
            0});
            this.numMin.Name = "numMin";
            this.numMin.Size = new System.Drawing.Size(63, 22);
            this.numMin.TabIndex = 0;
            // 
            // checkForced
            // 
            this.checkForced.Location = new System.Drawing.Point(6, 21);
            this.checkForced.Name = "checkForced";
            this.checkForced.Size = new System.Drawing.Size(154, 31);
            this.checkForced.TabIndex = 1;
            this.checkForced.Text = "Forcée";
            this.checkForced.UseVisualStyleBackColor = true;
            // 
            // checkEnabled
            // 
            this.checkEnabled.Location = new System.Drawing.Point(6, 58);
            this.checkEnabled.Name = "checkEnabled";
            this.checkEnabled.Size = new System.Drawing.Size(154, 31);
            this.checkEnabled.TabIndex = 2;
            this.checkEnabled.Text = "Autorisée/Activée";
            this.checkEnabled.UseVisualStyleBackColor = true;
            // 
            // checkAuto
            // 
            this.checkAuto.Location = new System.Drawing.Point(6, 132);
            this.checkAuto.Name = "checkAuto";
            this.checkAuto.Size = new System.Drawing.Size(75, 31);
            this.checkAuto.TabIndex = 3;
            this.checkAuto.Text = "Auto";
            this.checkAuto.UseVisualStyleBackColor = true;
            // 
            // numDuration
            // 
            this.numDuration.Location = new System.Drawing.Point(250, 261);
            this.numDuration.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.numDuration.Name = "numDuration";
            this.numDuration.Size = new System.Drawing.Size(63, 22);
            this.numDuration.TabIndex = 4;
            // 
            // numHour
            // 
            this.numHour.Location = new System.Drawing.Point(181, 197);
            this.numHour.Maximum = new decimal(new int[] {
            23,
            0,
            0,
            0});
            this.numHour.Name = "numHour";
            this.numHour.Size = new System.Drawing.Size(63, 22);
            this.numHour.TabIndex = 5;
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(166, 169);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(152, 25);
            this.label1.TabIndex = 6;
            this.label1.Text = "Heure de démarrage";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(161, 233);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(152, 25);
            this.label2.TabIndex = 7;
            this.label2.Text = "Durée (mins)";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
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
            this.checkedListDays.Location = new System.Drawing.Point(28, 169);
            this.checkedListDays.Name = "checkedListDays";
            this.checkedListDays.Size = new System.Drawing.Size(132, 140);
            this.checkedListDays.TabIndex = 9;
            // 
            // checkRemote
            // 
            this.checkRemote.Location = new System.Drawing.Point(6, 95);
            this.checkRemote.Name = "checkRemote";
            this.checkRemote.Size = new System.Drawing.Size(132, 31);
            this.checkRemote.TabIndex = 10;
            this.checkRemote.Text = "Remote activé";
            this.checkRemote.UseVisualStyleBackColor = true;
            // 
            // checkCmdRemote
            // 
            this.checkCmdRemote.Location = new System.Drawing.Point(144, 95);
            this.checkCmdRemote.Name = "checkCmdRemote";
            this.checkCmdRemote.Size = new System.Drawing.Size(153, 31);
            this.checkCmdRemote.TabIndex = 11;
            this.checkCmdRemote.Text = "Commande remote";
            this.checkCmdRemote.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(230, 22);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(83, 26);
            this.button1.TabIndex = 12;
            this.button1.Text = "Régler";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // CtrlPmpInfo
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.grpPmp);
            this.Name = "CtrlPmpInfo";
            this.Size = new System.Drawing.Size(333, 315);
            this.grpPmp.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.numMin)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numDuration)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numHour)).EndInit();
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
    }
}
