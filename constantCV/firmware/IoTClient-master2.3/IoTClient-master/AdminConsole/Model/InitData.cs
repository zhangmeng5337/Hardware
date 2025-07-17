using AdminConsole.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdminConsole.Model
{


    public class InitData
    {

        #region  基础数据
        public static List<KeyValue> danweiInputData = new List<KeyValue> {
                new KeyValue{ID= "0",Name="Pa"},
                new KeyValue{ID= "1",Name="KPa"},
                new KeyValue { ID = "2", Name = "MPa" },
                new KeyValue { ID = "3", Name = "mmH2O" },
                new KeyValue { ID = "4", Name = "mH2O" },
                new KeyValue { ID = "5", Name = "bar" },
                new KeyValue { ID = "6", Name = "psi" },
                new KeyValue { ID = "7", Name = "atm" },
                new KeyValue { ID = "8", Name = "kgf / cm2" },
                new KeyValue { ID = "9", Name = "mm" },
                new KeyValue { ID = "10", Name = "m" },
                new KeyValue { ID = "11", Name = "℃" },
                new KeyValue { ID = "12", Name = "℉" }
            };
        public static List<KeyValue> adcsulvData = new List<KeyValue> {
                new KeyValue{ID= "0",Name="10"},
                new KeyValue{ID= "1",Name="40"}
        };
        public static List<KeyValue> yiBiaoLeiXingData = new List<KeyValue> {
                new KeyValue{ID= "0",Name="温度"},
                new KeyValue{ID= "1",Name="压力"}
        };
        public static List<KeyValue> hengliuezhiData = new List<KeyValue> {
                new KeyValue{ID= "0.2",Name="0.2"},
                new KeyValue{ID= "0.3",Name="0.3"},
                new KeyValue{ID= "0.4",Name="0.4"},
                new KeyValue{ID= "0.5",Name="0.5"}
        };
        public static List<KeyValue> xiaozhundianData = new List<KeyValue> {
                new KeyValue{ID= "2",Name="2"},
                new KeyValue{ID= "3",Name="3"},
                new KeyValue{ID= "4",Name="4"},
                new KeyValue{ID= "5",Name="5"}
        };
        public static List<KeyValue> xiaoshudianData = new List<KeyValue> {
                new KeyValue{ID= "0",Name="0"},
                new KeyValue{ID= "1",Name="1"},
                new KeyValue{ID= "2",Name="2"},
                new KeyValue{ ID = "3", Name = "3" }
        };
        public static List<KeyValue> xiaoZhunWeiInputData = new List<KeyValue> {
                new KeyValue{ID= "0",Name="无检验"},
                new KeyValue{ID= "1",Name="奇检验"},
                new KeyValue{ ID = "2", Name = "偶检验" }
        };
        public static List<KeyValue> btlSelectData = new List<KeyValue> {
                new KeyValue{ID= "0",Name="1200"},
                new KeyValue{ID= "1",Name="2400"},
                new KeyValue { ID = "2", Name = "4800" },
                new KeyValue { ID = "3", Name = "9600" },
                new KeyValue { ID = "4", Name = "19200" },
                new KeyValue { ID = "5", Name = "38400" },
                new KeyValue { ID = "6", Name = "57600" },
                new KeyValue { ID = "7", Name = "115200" }
        };
        public static List<KeyValue> adcZhengYiData = new List<KeyValue> {
                new KeyValue{ID= "0",Name="1"},
                new KeyValue{ID= "1",Name="2"},
                new KeyValue{ID= "2",Name="4"},
                new KeyValue{ ID = "3", Name = "8" },
                new KeyValue{ ID = "4", Name = "16" },
                new KeyValue{ ID = "5", Name = "32" },
                new KeyValue{ ID = "6", Name = "64" },
                new KeyValue{ ID = "7", Name = "128" },
                new KeyValue{ ID = "8", Name = "256" }
        };

        public static List<KeyValue> lvbodengjiData = new List<KeyValue> {
                new KeyValue{ID= "0",Name="1"},
                new KeyValue{ID= "1",Name="2"},
                new KeyValue{ID= "2",Name="3"},
                new KeyValue{ID= "3",Name="4"},
                new KeyValue{ID= "4",Name="5"},
                new KeyValue{ID= "5",Name="6"},
                new KeyValue{ID= "6",Name="7"},
                new KeyValue{ID= "7",Name="8"},
                new KeyValue{ID= "8",Name="9"},
                new KeyValue{ID= "9",Name="10"}

        };

        public static List<RegisterDefinition> registers = new List<RegisterDefinition>
        {
            new RegisterDefinition(EnumDataId.实时值扩大1000倍, "实时值扩大1000倍", 0x0004, 1, DataType.Int16, "原始值"),
            new RegisterDefinition(EnumDataId.实时值扩大100倍, "实时值扩大100倍", 0x0000, 1, DataType.Int16, "原始值"),
            new RegisterDefinition(EnumDataId.实时值扩大10倍,"实时值扩大10倍", 0x0001, 1, DataType.Int16, "原始值"),         
            new RegisterDefinition(EnumDataId.浮点输出值,"浮点输出值", 0x0002, 2, DataType.Float, "工程值"),
            new RegisterDefinition(EnumDataId.浮点输出值低位,"浮点输出值低位", 0x0003, 2, DataType.Float, "工程值"),
            new RegisterDefinition(EnumDataId.原始浮点值,"原始浮点值", 0x0005, 2, DataType.Float, "原始浮点值"),
            new RegisterDefinition(EnumDataId.原始浮点值低位,"原始浮点值低位", 0x0006, 2, DataType.Float, "原始浮点值低位"),
            new RegisterDefinition(EnumDataId.量程零位,"量程零位", 0x0104, 2, DataType.Float, "浮点数"),
            new RegisterDefinition(EnumDataId.量程零位低位,"量程零位低位", 0x0105, 2, DataType.Float, "浮点数"),
            new RegisterDefinition(EnumDataId.量程满度,"量程满度", 0x0106, 2, DataType.Float, "浮点数"),
            new RegisterDefinition(EnumDataId.量程满度低位,"量程满度低位", 0x0107, 2, DataType.Float, "浮点数"),
            new RegisterDefinition(EnumDataId.量程零,"量程零位", 0x0104, 2, DataType.Float, "浮点数"),     
            new RegisterDefinition(EnumDataId.量程满,"量程满度", 0x0106, 2, DataType.Float, "浮点数"),     
            new RegisterDefinition(EnumDataId.偏移值,"偏移值", 0x0108, 2, DataType.Float, "浮点数"),
            new RegisterDefinition(EnumDataId.偏移值低位,"偏移值低位", 0x0109, 2, DataType.Float, "浮点数"),
            new RegisterDefinition(EnumDataId.设备地址,"设备地址", 0x012C, 1, DataType.UInt16, "1-247"),
            new RegisterDefinition(EnumDataId.波特率,"波特率", 0x012D, 1, DataType.UInt16, "0-7"),
            new RegisterDefinition(EnumDataId.校验位,"校验位", 0x012E, 1, DataType.UInt16, "0-2"),
            new RegisterDefinition(EnumDataId.小数位,"小数位", 0x012F, 1, DataType.UInt16, "0-3"),
            new RegisterDefinition(EnumDataId.单位,"单位", 0x000e, 1, DataType.UInt16, "0-12"),
            new RegisterDefinition(EnumDataId.速率,"速率", 0x0131, 1, DataType.UInt16, "10/40"),
            new RegisterDefinition(EnumDataId.清零状态,"清零状态", 0x0023, 1, DataType.UInt16, "1000/1001"),                 
            new RegisterDefinition(EnumDataId.单位1,"单位", 0x0130, 1, DataType.UInt16, "0-12"),
            new RegisterDefinition(EnumDataId.仪表类型,"仪表类型", 0x000b, 1, DataType.Int16, "仪表类型"),
            new RegisterDefinition(EnumDataId.pga放大倍数,"pga放大倍数", 0x000c, 1, DataType.Int16, "pga放大倍数"),
            new RegisterDefinition(EnumDataId.恒流值,"恒流值", 0x000d, 2, DataType.Float, "恒流值"),
            new RegisterDefinition(EnumDataId.coe高16,"coe高16", 0x000f, 2, DataType.Float, "coe高16"),
            new RegisterDefinition(EnumDataId.offsetH,"offsetH", 0x0011, 2, DataType.Float, "offsetH"),
            new RegisterDefinition(EnumDataId.工厂校准单位,"工厂校准单位", 0x000e, 1, DataType.Int16, "工厂校准单位"),
            new RegisterDefinition(EnumDataId.采集值1,"采集值1", 0x0001, 2, DataType.Float, "采集值1"),
            new RegisterDefinition(EnumDataId.采集值2,"采集值2", 0x0001, 2, DataType.Float, "采集值2"),
            new RegisterDefinition(EnumDataId.采集值3,"采集值3", 0x0001, 2, DataType.Float, "采集值3"),
            new RegisterDefinition(EnumDataId.采集值4,"采集值4", 0x0001, 2, DataType.Float, "采集值4"),
            new RegisterDefinition(EnumDataId.采集值5,"采集值5", 0x0001, 2, DataType.Float, "采集值5"),
            new RegisterDefinition(EnumDataId.工厂模式,"工厂模式", 0x001d, 1, DataType.Int16, "工厂模式",1),
            new RegisterDefinition(EnumDataId.校准系数1,"校准系数1", 0x0013, 2, DataType.Float, "校准系数1"),
            new RegisterDefinition(EnumDataId.校准系数2,"校准系数2", 0x0015, 2, DataType.Float, "校准系数2"),
            new RegisterDefinition(EnumDataId.校准系数3,"校准系数3", 0x0017, 2, DataType.Float, "校准系数3"),
            new RegisterDefinition(EnumDataId.校准系数4,"校准系数4", 0x0019, 2, DataType.Float, "校准系数4"),
            new RegisterDefinition(EnumDataId.校准系数5,"校准系数5", 0x0034, 2, DataType.Float, "校准系数5"),
            new RegisterDefinition(EnumDataId.校准采集值1,"校准采集值1", 0x001e, 2, DataType.Float, "校准采集值1"),
            new RegisterDefinition(EnumDataId.校准采集值2,"校准采集值2", 0x0020, 2, DataType.Float, "校准采集值2"),
            new RegisterDefinition(EnumDataId.校准采集值3,"校准采集值3", 0x0022, 2, DataType.Float, "校准采集值3"),
            new RegisterDefinition(EnumDataId.校准采集值4,"校准采集值4", 0x0024, 2, DataType.Float, "校准采集值4"),
            new RegisterDefinition(EnumDataId.校准采集值5,"校准采集值5", 0x0026, 2, DataType.Float, "校准采集值5"),
            new RegisterDefinition(EnumDataId.校准标准值1,"校准标准值1", 0x0028, 2, DataType.Float, "校准标准值1"),
            new RegisterDefinition(EnumDataId.校准标准值2,"校准标准值2", 0x002a, 2, DataType.Float, "校准标准值2"),
            new RegisterDefinition(EnumDataId.校准标准值3,"校准标准值3", 0x002c, 2, DataType.Float, "校准标准值3"),
            new RegisterDefinition(EnumDataId.校准标准值4,"校准标准值4", 0x002e, 2, DataType.Float, "校准标准值4"),
            new RegisterDefinition(EnumDataId.校准标准值5,"校准标准值5", 0x0030, 2, DataType.Float, "校准标准值5"),
            new RegisterDefinition(EnumDataId.恢复出厂设置,"恢复出厂设置", 0x001d, 1, DataType.Int16, "工厂模式",2),
            new RegisterDefinition(EnumDataId.屏蔽零点,"屏蔽零点", 0x0032, 1, DataType.Float, "屏蔽零点"),
            new RegisterDefinition(EnumDataId.保存参数指令,"保存参数指令", 0x001b, 1, DataType.Int16, "保存参数指令",1),
            new RegisterDefinition(EnumDataId.滤波等级,"滤波等级", 0x0036, 1, DataType.Int16, "滤波等级"),
        };

        //public static List<RegisterDefinition> JiCunQiQuanBu = new List<RegisterDefinition>
        //{
           
        //    new RegisterDefinition(EnumDataId.偏移值,"偏移值", 0x0108, 2, DataType.Float, "浮点数"),
        //    new RegisterDefinition(EnumDataId.设备地址,"设备地址", 0x012C, 1, DataType.UInt16, "1-247"),
        //    new RegisterDefinition(EnumDataId.波特率,"波特率", 0x012D, 1, DataType.UInt16, "0-7"),
        //    new RegisterDefinition(EnumDataId.校验位,"校验位", 0x012E, 1, DataType.UInt16, "0-2"),
        //    new RegisterDefinition(EnumDataId.小数位,"小数位", 0x012F, 1, DataType.UInt16, "0-3"),
        //    new RegisterDefinition(EnumDataId.单位1,"单位", 0x0130, 1, DataType.UInt16, "0-12"),
        //    new RegisterDefinition(EnumDataId.速率,"速率", 0x0131, 1, DataType.UInt16, "10/40"),            
        //    new RegisterDefinition(EnumDataId.量程零,"量程零位", 0x0104, 2, DataType.Float, "浮点数"),
        //    new RegisterDefinition(EnumDataId.量程满,"量程满度", 0x0106, 2, DataType.Float, "浮点数"),
         
        //};

        public static List<EnumDataId> SbDzRegisters = new List<EnumDataId>
        {
            EnumDataId.实时值扩大1000倍,
            EnumDataId.实时值扩大100倍,
            EnumDataId.实时值扩大10倍,
            EnumDataId.浮点输出值,
            EnumDataId.原始浮点值,
            EnumDataId.量程零位,
           
            EnumDataId.量程满度,
        
            EnumDataId.偏移值,
     
            EnumDataId.设备地址,
            EnumDataId.波特率,
            EnumDataId.校验位,
            EnumDataId.小数位,
            EnumDataId.单位1,
            EnumDataId.速率,
            EnumDataId.清零状态
        };
 
        //public static List<RegisterDefinition> YiBiaoXinXi = new List<RegisterDefinition>
        //{
        //    new RegisterDefinition(EnumDataId.量程零位,"量程零位", 0x0104, 2, DataType.Float, "浮点数"),
        //    new RegisterDefinition(EnumDataId.量程满度,"量程满度", 0x0106, 2, DataType.Float, "浮点数"),
        //    new RegisterDefinition(EnumDataId.单位,"单位", 0x0130, 1, DataType.UInt16, "0-12"),
        //    new RegisterDefinition(EnumDataId.实时值扩大100倍, "实时值扩大100倍", 0x0000, 1, DataType.Int16, "原始值"),
        //    new RegisterDefinition(EnumDataId.实时值扩大10倍,"实时值扩大10倍", 0x0001, 1, DataType.Int16, "原始值"),
        //    new RegisterDefinition(EnumDataId.浮点输出值,"浮点输出值", 0x0002, 2, DataType.Float, "工程值"),
        //};
       
        public static List<EnumDataId> YiBiaoXinXiIds = new List<EnumDataId>
        {          
            EnumDataId.单位,             
        };
        public static List<EnumDataId> CmdYiBiaoXinXiIds = new List<EnumDataId>
        {
            EnumDataId.单位,
            EnumDataId.量程零位,
            EnumDataId.量程满度,            
        };
     

        #endregion


        //用户设置
        //public static List<RegisterDefinition> YongHuSheZhi = new List<RegisterDefinition>
        //{             
        //    new RegisterDefinition(EnumDataId.设备地址,"地  址", 0x012C, 1, DataType.UInt16, "1-247"),
        //    new RegisterDefinition(EnumDataId.波特率,"波特率", 0x012D, 1, DataType.UInt16, "0-7"),
        //    new RegisterDefinition(EnumDataId.校验位,"校准位", 0x012E, 1, DataType.UInt16, "0-2"),
        //    new RegisterDefinition(EnumDataId.速率,"ADC速率", 0x0131, 1, DataType.UInt16, "10/40"),
        //    new RegisterDefinition(EnumDataId.单位1,"单位", 0x0130, 1, DataType.UInt16, "0-12"),
        //    new RegisterDefinition(EnumDataId.小数位,"小数位", 0x012F, 1, DataType.UInt16, "0-3"),
        //    new RegisterDefinition(EnumDataId.偏移值,"偏移值", 0x0108, 2, DataType.Float, "浮点数"),                                    
        //};
        public static List<EnumDataId> YongHuSheZhiIds = new List<EnumDataId>
        {
          EnumDataId.设备地址,
           EnumDataId.波特率,
            EnumDataId.校验位,
           EnumDataId.速率,
           EnumDataId.单位1,
           EnumDataId.小数位
        };


        //public static List<RegisterDefinition> JiaoZhunDuQu = new List<RegisterDefinition>
        //{
        //    new RegisterDefinition(EnumDataId.量程零,"量程零位", 0x0104, 2, DataType.Float, "浮点数"),
        //    new RegisterDefinition(EnumDataId.量程满,"量程满度", 0x0106, 2, DataType.Float, "浮点数"),
        //    new RegisterDefinition(EnumDataId.单位2,"单位", 0x0130, 1, DataType.UInt16, "0-12"),            
        //};
       


        //public static List<RegisterDefinition> JiaoZhunCmdDuQu = new List<RegisterDefinition>
        //{
        //    new RegisterDefinition(EnumDataId.量程零,"量程零位", 0x0104, 2, DataType.Float, "浮点数"),
        //    new RegisterDefinition(EnumDataId.量程满,"量程满度", 0x0106, 2, DataType.Float, "浮点数"),
        //    new RegisterDefinition(EnumDataId.仪表类型,"仪表类型", 0x000b, 1, DataType.Int16, "仪表类型"),
        //    new RegisterDefinition(EnumDataId.pga放大倍数,"pga放大倍数", 0x000c, 1, DataType.Int16, "pga放大倍数"),
        //    new RegisterDefinition(EnumDataId.恒流值,"恒流值", 0x000d, 2, DataType.Float, "恒流值"),
        //    new RegisterDefinition(EnumDataId.coe高16,"coe高16", 0x000f, 2, DataType.Float, "coe高16"),
        //    new RegisterDefinition(EnumDataId.offsetH,"offsetH", 0x0011, 2, DataType.Float, "offsetH"),
        //    new RegisterDefinition(EnumDataId.工厂校准单位,"工厂校准单位", 0x000e, 1, DataType.Int16, "工厂校准单位"),
        //    new RegisterDefinition(EnumDataId.采集值1,"采集值1", 0x0001, 2, DataType.Float, "采集值1"),
        //    new RegisterDefinition(EnumDataId.采集值2,"采集值2", 0x0001, 2, DataType.Float, "采集值2"),
        //    new RegisterDefinition(EnumDataId.采集值3,"采集值3", 0x0001, 2, DataType.Float, "采集值3"),
        //    new RegisterDefinition(EnumDataId.采集值4,"采集值4", 0x0001, 2, DataType.Float, "采集值4"),
        //    new RegisterDefinition(EnumDataId.采集值5,"采集值5", 0x0001, 2, DataType.Float, "采集值5"),
        //};

        public static List<EnumDataId> CmdJiaoZhunDuQuIds = new List<EnumDataId>
        {
            EnumDataId.量程零,
            EnumDataId.量程满,
            EnumDataId.仪表类型,
            EnumDataId.pga放大倍数,
            EnumDataId.恒流值,
            EnumDataId.coe高16,
            EnumDataId.offsetH,
            EnumDataId.屏蔽零点,
            EnumDataId.工厂校准单位,
            EnumDataId.校准采集值1,
            EnumDataId.校准采集值2,
            EnumDataId.校准采集值3,
            EnumDataId.校准采集值4,
            EnumDataId.校准采集值5,
            EnumDataId.校准标准值1,
            EnumDataId.校准标准值2,
            EnumDataId.校准标准值3,
            EnumDataId.校准标准值4,
            EnumDataId.校准标准值5,
            EnumDataId.滤波等级
            

        };

        public static List<EnumDataId> CmdUserJiaoZhunDuQuIds = new List<EnumDataId>
        {
             
           
            EnumDataId.校准采集值1,
            EnumDataId.校准采集值2,
            EnumDataId.校准采集值3,
            EnumDataId.校准采集值4,
            EnumDataId.校准采集值5,
            EnumDataId.校准标准值1,
            EnumDataId.校准标准值2,
            EnumDataId.校准标准值3,
            EnumDataId.校准标准值4,
            EnumDataId.校准标准值5,
            EnumDataId.工厂校准单位


        };

        public static List<EnumDataId> CmdUserCanShuDuQuIds = new List<EnumDataId>
        {


            EnumDataId.校准采集值1,
            EnumDataId.校准采集值2,
            EnumDataId.校准采集值3,
            EnumDataId.校准采集值4,
            EnumDataId.校准采集值5,
            EnumDataId.校准标准值1,
            EnumDataId.校准标准值2,
            EnumDataId.校准标准值3,
            EnumDataId.校准标准值4,
            EnumDataId.校准标准值5,
           


        };




        //public static List<RegisterDefinition> JiaoZhunCmdXieRu = new List<RegisterDefinition>
        //{
        //    new RegisterDefinition(EnumDataId.量程零,"量程零位", 0x0104, 2, DataType.Float, "浮点数"),
        //    new RegisterDefinition(EnumDataId.量程满,"量程满度", 0x0106, 2, DataType.Float, "浮点数"),
        //    new RegisterDefinition(EnumDataId.仪表类型,"仪表类型", 0x000b, 1, DataType.Int16, "仪表类型"),
        //    new RegisterDefinition(EnumDataId.pga放大倍数,"pga放大倍数", 0x000c, 1, DataType.Int16, "pga放大倍数"),
        //    new RegisterDefinition(EnumDataId.恒流值,"恒流值", 0x000d, 2, DataType.Float, "恒流值"),
        //    new RegisterDefinition(EnumDataId.工厂校准单位,"工厂校准单位", 0x000e, 1, DataType.Int16, "工厂校准单位"),
        //    new RegisterDefinition(EnumDataId.coe高16,"coe高16", 0x000f, 2, DataType.Float, "coe高16"),
        //    new RegisterDefinition(EnumDataId.offsetH,"offsetH", 0x0011, 2, DataType.Float, "offsetH"),
        //    new RegisterDefinition(EnumDataId.校准系数1,"校准系数1", 0x0013, 2, DataType.Float, "校准系数1"),
        //    new RegisterDefinition(EnumDataId.校准系数2,"校准系数2", 0x0015, 2, DataType.Float, "校准系数2"),
        //    new RegisterDefinition(EnumDataId.校准系数3,"校准系数3", 0x0017, 2, DataType.Float, "校准系数3"),
        //    new RegisterDefinition(EnumDataId.校准系数4,"校准系数4", 0x0019, 2, DataType.Float, "校准系数4"),
        //    new RegisterDefinition(EnumDataId.保存参数指令,"保存参数指令", 0x001b, 1, DataType.Int16, "保存参数指令",1),

        //};

        public static List<EnumDataId> JiaoZhunCmdXieRuIds = new List<EnumDataId>
        {        
           EnumDataId.仪表类型, 
           EnumDataId.pga放大倍数, 
           EnumDataId.恒流值, 
           EnumDataId.工厂校准单位,
           EnumDataId.屏蔽零点,
           EnumDataId.coe高16, 
           EnumDataId.offsetH, 
           EnumDataId.校准系数1, 
           EnumDataId.校准系数2, 
           EnumDataId.校准系数3, 
           EnumDataId.校准系数4,
            EnumDataId.校准采集值1,
            EnumDataId.校准采集值2,
            EnumDataId.校准采集值3,
            EnumDataId.校准采集值4,
            EnumDataId.校准采集值5,
            EnumDataId.校准标准值1,
            EnumDataId.校准标准值2,
            EnumDataId.校准标准值3,
            EnumDataId.校准标准值4,
            EnumDataId.校准标准值5,
            EnumDataId.滤波等级
            
        };

        public static List<EnumDataId> UserJiaoZhunCmdXieRuIds = new List<EnumDataId>
        {
           
           EnumDataId.校准系数1,
           EnumDataId.校准系数2,
           EnumDataId.校准系数3,
           EnumDataId.校准系数4,
           EnumDataId.校准系数5,
            EnumDataId.校准采集值1,
            EnumDataId.校准采集值2,
          
            EnumDataId.校准标准值1,
            EnumDataId.校准标准值2,
      

        };



        //public static List<RegisterDefinition> GongChangCmdXieRu = new List<RegisterDefinition>
        //{
        //    new RegisterDefinition(EnumDataId.工厂模式,"工厂模式", 0x001d, 1, DataType.Int16, "工厂模式",1),
        //    new RegisterDefinition(EnumDataId.量程零,"量程零位", 0x0104, 2, DataType.Float, "浮点数"),
        //    new RegisterDefinition(EnumDataId.量程满,"量程满度", 0x0106, 2, DataType.Float, "浮点数"),
        //    new RegisterDefinition(EnumDataId.仪表类型,"仪表类型", 0x000b, 1, DataType.Int16, "仪表类型"),     
        //    new RegisterDefinition(EnumDataId.pga放大倍数,"pga放大倍数", 0x000c, 1, DataType.Int16, "pga放大倍数"),
        //    new RegisterDefinition(EnumDataId.恒流值,"恒流值", 0x000d, 2, DataType.Float, "恒流值"),
        //    new RegisterDefinition(EnumDataId.工厂校准单位,"工厂校准单位", 0x000e, 1, DataType.Int16, "工厂校准单位"),
        //    new RegisterDefinition(EnumDataId.coe高16,"coe高16", 0x000f, 2, DataType.Float, "coe高16"),
        //    new RegisterDefinition(EnumDataId.offsetH,"offsetH", 0x0011, 2, DataType.Float, "offsetH"),
        //    new RegisterDefinition(EnumDataId.校准系数1,"校准系数1", 0x0013, 2, DataType.Float, "校准系数1"),
        //    new RegisterDefinition(EnumDataId.校准系数2,"校准系数2", 0x0015, 2, DataType.Float, "校准系数2"),
        //    new RegisterDefinition(EnumDataId.校准系数3,"校准系数3", 0x0017, 2, DataType.Float, "校准系数3"),
        //    new RegisterDefinition(EnumDataId.校准系数4,"校准系数4", 0x0019, 2, DataType.Float, "校准系数4"),
        //    new RegisterDefinition(EnumDataId.保存参数指令,"保存参数指令", 0x001b, 1, DataType.Int16, "保存参数指令",1),

        //};









    }


}
