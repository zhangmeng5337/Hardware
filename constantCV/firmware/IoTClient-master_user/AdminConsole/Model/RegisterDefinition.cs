using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AdminConsole.Model
{
	 

    public class RegisterDefinition
    {
        public EnumDataId Id  { get; }

       

        public string Name { get; set; }

        public object val { get; set; }

        public string CustomizeAddress { get; set; }

        public ushort Address { get; }
        public ushort Length { get; }
        public DataType DataType { get; }
        public string Description { get; set; }

        public RegisterDefinition(EnumDataId id ,string name, int address, int length,
            DataType type, string desc)
        {
            Id = id;
            Name = name;
            Address = (ushort)address;
            Length = (ushort)length;
            DataType = type;
            Description = desc;
        }
        public RegisterDefinition(  EnumDataId id, string name, int address, int length,
            DataType type, string desc,object value)
        {
            Id = id;
            Name = name;
            Address = (ushort)address;
            Length = (ushort)length;
            DataType = type;
            Description = desc;
            val = value;
        }
        public RegisterDefinition( )
        {
            
        }
    }

    public enum DataType { IntFloat32, Int32, Int16, UInt16, Float, Sting }

    public enum EnumDataId
    {
        实时值扩大1000倍,
        实时值扩大100倍,
        实时值扩大10倍, 
        浮点输出值,
        浮点输出值低位,
        原始浮点值,
        原始浮点值低位,
        量程零位,
        量程零位低位,      
        量程满度,
        量程满度低位,
        偏移值,
        偏移值低位,
        设备地址,
        波特率,
        校验位,
        小数位,
        单位,
        速率,
        清零状态,
        单位1,
        量程零,
        量程满,                     
        仪表类型,
        恒流值,
        pga放大倍数,
        工厂校准单位,
        coe高16,
        offsetH,
        校准系数1,
        校准系数2,
        校准系数3,
        校准系数4,
        校准系数5,
        采集值1,
        采集值2,
        采集值3,
        采集值4,
        采集值5,
        校准采集值1,
        校准采集值2,
        校准采集值3,
        校准采集值4,
        校准采集值5,
        校准标准值1,
        校准标准值2,
        校准标准值3,
        校准标准值4,
        校准标准值5,
        保存参数指令,   
        恢复出厂设置,
        屏蔽零点,
        工厂模式,

        滤波等级,
        用户校准标准值1,
        用户校准采集值1,
        用户校准标准值2,
        用户校准采集值2
    }


    public struct value_string
    {
        public string cal_usradc1;
        public string cal_usradc2;
        public string cal_usrval1;
        public string cal_usrval2;
        public string cal_adc1;
        public string cal_adc2;
        public string cal_adc3;
        public string cal_adc4;
        public string cal_adc5;
        public string cal_val1;
        public string cal_val2;
        public string cal_val3;
        public string cal_val4;
        public string cal_val5;
        public string selectValue;
    }
 
 
 
 

}
