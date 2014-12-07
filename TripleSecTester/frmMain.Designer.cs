namespace TripleSecTester
{
    partial class frmMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.txtOutput = new System.Windows.Forms.TextBox();
            this.btnGO = new System.Windows.Forms.Button();
            this.btnKATS = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // txtOutput
            // 
            this.txtOutput.Location = new System.Drawing.Point(14, 47);
            this.txtOutput.MaxLength = 327671;
            this.txtOutput.Multiline = true;
            this.txtOutput.Name = "txtOutput";
            this.txtOutput.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtOutput.Size = new System.Drawing.Size(865, 381);
            this.txtOutput.TabIndex = 0;
            // 
            // btnGO
            // 
            this.btnGO.Location = new System.Drawing.Point(14, 12);
            this.btnGO.Name = "btnGO";
            this.btnGO.Size = new System.Drawing.Size(358, 27);
            this.btnGO.TabIndex = 1;
            this.btnGO.Text = "FREE FORM TEST (look for example code in here)";
            this.btnGO.UseVisualStyleBackColor = true;
            this.btnGO.Click += new System.EventHandler(this.btnGO_Click);
            // 
            // btnKATS
            // 
            this.btnKATS.Location = new System.Drawing.Point(482, 12);
            this.btnKATS.Name = "btnKATS";
            this.btnKATS.Size = new System.Drawing.Size(395, 27);
            this.btnKATS.TabIndex = 2;
            this.btnKATS.Text = "REGIMENTED KNOWN ANSWER TESTS";
            this.btnKATS.UseVisualStyleBackColor = true;
            this.btnKATS.Click += new System.EventHandler(this.btnKATS_Click);
            // 
            // frmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(889, 443);
            this.Controls.Add(this.btnKATS);
            this.Controls.Add(this.btnGO);
            this.Controls.Add(this.txtOutput);
            this.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Name = "frmMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "TripleSec Tester";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txtOutput;
        private System.Windows.Forms.Button btnGO;
        private System.Windows.Forms.Button btnKATS;
    }
}

