/******************************************************************************
 * SunnyUI 开源控件库、工具类库、扩展类库、多页面开发框架。
 * CopyRight (C) 2012-2020 ShenYongHua(沈永华).
 * QQ群：56829229 QQ：17612584 EMail：SunnyUI@qq.com
 *
 * Blog:   https://www.cnblogs.com/yhuse
 * Gitee:  https://gitee.com/yhuse/SunnyUI
 * GitHub: https://github.com/yhuse/SunnyUI
 *
 * SunnyUI.dll can be used for free under the GPL-3.0 license.
 * If you use this code, please keep this note.
 * 如果您使用此代码，请保留此说明。
 ******************************************************************************
 * 文件名称: UISymbolButton.cs
 * 文件说明: 字体图标按钮
 * 当前版本: V2.2
 * 创建日期: 2020-01-01
 *
 * 2020-01-01: V2.2.0 增加文件说明
 * 2020-07-26: V2.2.6 增加Image属性，增加图片和文字的摆放位置
******************************************************************************/

using System;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Design;
using System.Drawing.Drawing2D;
using System.Management.Instrumentation;
using System.Windows.Forms;

namespace Sunny.UI
{
    [DefaultEvent("Click")]
    [DefaultProperty("Text")]
    public sealed class UISymbolButton : UIButton
    {
        private int _symbolSize = 24;
        private int _imageInterval = 2;

        public UISymbolButton()
        {
            ShowText = false;
        }

        [DefaultValue(24)]
        [Description("字体图标大小"), Category("SunnyUI")]
        public int SymbolSize
        {
            get => _symbolSize;
            set
            {
                _symbolSize = Math.Max(value, 16);
                _symbolSize = Math.Min(value, 64);
                Invalidate();
            }
        }

        [DefaultValue(null)]
        [Description("图片"), Category("SunnyUI")]
        public Image Image { get; set; }

        private ContentAlignment imageAlign = ContentAlignment.MiddleCenter;

        [DefaultValue(ContentAlignment.MiddleCenter)]
        [Description("图片放置位置"), Category("SunnyUI")]
        public ContentAlignment ImageAlign
        {
            get => imageAlign;
            set
            {
                imageAlign = value;
                Invalidate();
            }
        }

        [DefaultValue(2)]
        [Description("图片文字间间隔"), Category("SunnyUI")]
        public int ImageInterval
        {
            get => _imageInterval;
            set
            {
                _imageInterval = Math.Max(0, value);
                Invalidate();
            }
        }

        private bool _isCircle;

        [DefaultValue(false)]
        [Description("是否是圆形"), Category("SunnyUI")]
        public bool IsCircle
        {
            get => _isCircle;
            set
            {
                _isCircle = value;
                if (value)
                {
                    Text = "";
                }
                else
                {
                    Invalidate();
                }
            }
        }

        private int _symbol = FontAwesomeIcons.fa_check;

        [DesignerSerializationVisibility(DesignerSerializationVisibility.Visible)]
        [Editor(typeof(UIImagePropertyEditor), typeof(UITypeEditor))]
        [DefaultValue(61452)]
        [Description("字体图标"), Category("SunnyUI")]
        public int Symbol
        {
            get => _symbol;
            set
            {
                _symbol = value;
                Invalidate();
            }
        }

        protected override void OnPaintFill(Graphics g, GraphicsPath path)
        {
            if (IsCircle)
            {
                int size = Math.Min(Width, Height) - 2 - CircleRectWidth;
                g.FillEllipse(GetFillColor(), (Width - size) / 2.0f, (Height - size) / 2.0f, size, size);
            }
            else
            {
                base.OnPaintFill(g, path);
            }
        }

        private int circleRectWidth = 1;

        [DefaultValue(1)]
        [Description("圆形边框大小"), Category("SunnyUI")]
        public int CircleRectWidth
        {
            get => circleRectWidth;
            set
            {
                circleRectWidth = value;
                Invalidate();
            }
        }

        protected override void OnPaintRect(Graphics g, GraphicsPath path)
        {
            if (IsCircle)
            {
                int size = Math.Min(Width, Height) - 2 - CircleRectWidth;
                using (Pen pn = new Pen(GetRectColor(), CircleRectWidth))
                {
                    g.SetHighQuality();
                    g.DrawEllipse(pn, (Width - size) / 2.0f, (Height - size) / 2.0f, size, size);
                    g.SetDefaultQuality();
                }
            }
            else
            {
                base.OnPaintRect(g, path);
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            //重绘父类
            base.OnPaint(e);

            SizeF ImageSize = new SizeF(0, 0);
            if (Symbol > 0)
                ImageSize = e.Graphics.GetFontImageSize(Symbol, SymbolSize);
            if (Image != null)
                ImageSize = Image.Size;




            //字体图标
            Color color = GetForeColor();
           

            var FontSize = Font.Size ;
            FontSize = this.Height / 35.0f * FontSize;

            var imgSizeWidth = 23.621f;
            var imgInterval = 2;


            var Font1 = new Font(Font.FontFamily, FontSize, Font.Style); // 设置字符的大小为原来的两倍
            SizeF TextSize = e.Graphics.MeasureString(Text, Font1);

            string lexing = "";
            string shuju = "";
            Color jklxcolor = new Color();
            Color jksjcolor = new Color();
            if (this.Tag != null)
            {
                var arr = this.Tag.ToString().Split("_");
                if (arr.Length > 2) {

                    try
                    {
                        var lxys = arr[1].Split('&');
                        var sjys = arr[2].Split('&');

                        if (lxys[1].StartsWith("ff"))
                        {
                            jklxcolor = ColorTranslator.FromHtml("#" + lxys[1].Substring(2));
                        }
                        else
                        {
                            jklxcolor = ColorTranslator.FromHtml(lxys[1]);
                        }
                        if (sjys[1].StartsWith("ff"))
                        {
                            jksjcolor = ColorTranslator.FromHtml("#" + sjys[1].Substring(2));
                        }
                        else
                        {
                            jksjcolor = ColorTranslator.FromHtml(sjys[1]);
                        }

                        lexing = lxys[0];

                        shuju = sjys[0];

                     
                    }
                    catch
                    {

                    }

                    
                }                
            }

            var charFont = new Font(Font.FontFamily, FontSize / 1.5f, Font.Style); // 设置字符的大小为原来的两倍
            SizeF charSize1 = e.Graphics.MeasureString(lexing, charFont);

 
            if (ImageAlign == ContentAlignment.MiddleCenter && TextAlign == ContentAlignment.MiddleCenter)
            {
                if (ImageSize.Width.Equals(0))
                {
                    if (TextSize.Width > 0) {

                        if (Symbol > -1){
                            float allWidth = imgSizeWidth + imgInterval + TextSize.Width + charSize1.Width;
                            e.Graphics.DrawString(Text, Font1, color, (Width - allWidth) / 2.0f + imgSizeWidth + imgInterval, (Height - TextSize.Height) / 2.0f);
                            e.Graphics.DrawString(lexing, charFont, jklxcolor, (Width - allWidth) / 2.0f + imgSizeWidth + imgInterval + TextSize.Width, (Height) / 2.0f - TextSize.Height / 1.5f);
                            e.Graphics.DrawString(shuju, charFont, jksjcolor, (Width - allWidth) / 2.0f + imgSizeWidth + imgInterval + TextSize.Width, (Height) / 2.0f);
                        }
                        else {
                            float allWidth = TextSize.Width + charSize1.Width;
                            e.Graphics.DrawString(Text, Font1, color, (Width - allWidth) / 2.0f + ImageSize.Width + ImageInterval, (Height - TextSize.Height) / 2.0f);
                            e.Graphics.DrawString(lexing, charFont, Brushes.Red, (Width - allWidth) / 2.0f + TextSize.Width, (Height) / 2.0f - TextSize.Height / 1.5f);
                            e.Graphics.DrawString(shuju, charFont, Brushes.Yellow, (Width - allWidth) / 2.0f + TextSize.Width, (Height) / 2.0f);
                        }
 
                    }
                }
                else if (TextSize.Width.Equals(0))
                {
                    if (ImageSize.Width > 0)
                    {
                        if (Symbol > 0 && Image == null)
                        {
                            e.Graphics.DrawFontImage(Symbol, SymbolSize, color,
                                new RectangleF((Width - ImageSize.Width) / 2.0f, (Height - ImageSize.Height) / 2.0f,
                                    ImageSize.Width, ImageSize.Height));
                        }

                        if (Image != null)
                        {
                            e.Graphics.DrawImage(Image, (Width - Image.Width) / 2.0f, (Height - Image.Height) / 2.0f,
                                ImageSize.Width, ImageSize.Height);
                        }
                    }
                }
                else
                {
 
                    float allWidth = imgSizeWidth + ImageInterval + TextSize.Width+ charSize1.Width;

                    if (Symbol > 0 && Image == null)
                    {
                        e.Graphics.DrawFontImage(Symbol, SymbolSize, color,
                            new RectangleF((Width - allWidth) / 2.0f, (Height - ImageSize.Height) / 2.0f, ImageSize.Width, ImageSize.Height));
                       
                    }

                    if (Image != null)
                    {
                        e.Graphics.DrawImage(Image, (Width - allWidth) / 2.0f, (Height - ImageSize.Height) / 2.0f,
                            ImageSize.Width, ImageSize.Height);
                    }

                    e.Graphics.DrawString(Text, Font1, color, (Width - allWidth) / 2.0f + imgSizeWidth + ImageInterval, (Height - TextSize.Height) / 2.0f);

                    e.Graphics.DrawString(lexing, charFont, jklxcolor, (Width - allWidth) / 2.0f + imgSizeWidth + ImageInterval + TextSize.Width, (Height) / 2.0f - TextSize.Height / 1.5f);
                    e.Graphics.DrawString(shuju, charFont, jksjcolor, (Width - allWidth) / 2.0f + imgSizeWidth + ImageInterval + TextSize.Width, (Height) / 2.0f);

                }
            }
            else
            {
                float left = 0;
                float top = 0;

                if (ImageSize.Width > 0)
                {
                    switch (ImageAlign)
                    {
                        case ContentAlignment.TopLeft:
                            left = Padding.Left;
                            top = Padding.Top;
                            break;

                        case ContentAlignment.TopCenter:
                            left = (Width - ImageSize.Width) / 2.0f;
                            top = Padding.Top;
                            break;

                        case ContentAlignment.TopRight:
                            left = Width - Padding.Right - ImageSize.Width;
                            top = Padding.Top;
                            break;

                        case ContentAlignment.MiddleLeft:
                            left = Padding.Left;
                            top = (Height - ImageSize.Height) / 2.0f;
                            break;

                        case ContentAlignment.MiddleCenter:
                            left = (Width - ImageSize.Width) / 2.0f;
                            top = (Height - ImageSize.Height) / 2.0f;
                            break;

                        case ContentAlignment.MiddleRight:
                            left = Width - Padding.Right - ImageSize.Width;
                            top = (Height - ImageSize.Height) / 2.0f;
                            break;

                        case ContentAlignment.BottomLeft:
                            left = Padding.Left;
                            top = Height - Padding.Bottom - ImageSize.Height;
                            break;

                        case ContentAlignment.BottomCenter:
                            left = (Width - ImageSize.Width) / 2.0f;
                            top = Height - Padding.Bottom - ImageSize.Height;
                            break;

                        case ContentAlignment.BottomRight:
                            left = Width - Padding.Right - ImageSize.Width;
                            top = Height - Padding.Bottom - ImageSize.Height;
                            break;
                    }

                    if (Symbol > 0 && Image == null)
                    {
                        e.Graphics.DrawFontImage(Symbol, SymbolSize, color,
                            new RectangleF(left, top, ImageSize.Width, ImageSize.Height));
                    }

                    if (Image != null)
                    {
                      //  e.Graphics.DrawImage(Image, left, top, ImageSize.Width, ImageSize.Height);
                        e.Graphics.DrawImage(Image, new Rectangle(0, 0, this.Width, this.Height));

                    }                 
                }

                left = 0;
                top = 0;
                if (TextSize.Width > 0)
                {
                    switch (TextAlign)
                    {
                        case ContentAlignment.TopLeft:
                            left = Padding.Left;
                            top = Padding.Top;
                            break;

                        case ContentAlignment.TopCenter:
                            left = (Width - TextSize.Width) / 2.0f;
                            top = Padding.Top;
                            break;

                        case ContentAlignment.TopRight:
                            left = Width - Padding.Right - TextSize.Width;
                            top = Padding.Top;
                            break;

                        case ContentAlignment.MiddleLeft:
                            left = Padding.Left;
                            top = (Height - TextSize.Height) / 2.0f;
                            break;

                        case ContentAlignment.MiddleCenter:
                            left = (Width - TextSize.Width) / 2.0f;
                            top = (Height - TextSize.Height) / 2.0f;
                            break;

                        case ContentAlignment.MiddleRight:
                            left = Width - Padding.Right - TextSize.Width;
                            top = (Height - TextSize.Height) / 2.0f;
                            break;

                        case ContentAlignment.BottomLeft:
                            left = Padding.Left;
                            top = Height - Padding.Bottom - TextSize.Height;
                            break;

                        case ContentAlignment.BottomCenter:
                            left = (Width - TextSize.Width) / 2.0f;
                            top = Height - Padding.Bottom - TextSize.Height;
                            break;

                        case ContentAlignment.BottomRight:
                            left = Width - Padding.Right - TextSize.Width;
                            top = Height - Padding.Bottom - TextSize.Height;
                            break;
                    }

                    
                    e.Graphics.DrawString(Text, Font1, color, (Width - TextSize.Width) / 2.0f, (Height - TextSize.Height) / 2.0f);
                 
                }
            }
        }
    }
}