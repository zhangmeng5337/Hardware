using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TemperatureRFIDChartWin
{
    public class ConstCode
    {
        /// <summary>
        /// 单包数据包开始字节
        /// </summary>
        public const byte FRAME_BEGIN = 0xbb;

        /// <summary>
        /// 单包数据包结束帧字节
        /// </summary>
        public const byte FRAME_END = 0x7e;


        /// <summary>
        /// 命令帧字节
        /// </summary>
        public const byte FRAME_TYPE_CMD = 0x00;

        /// <summary>
        /// 响应帧字节
        /// </summary>
        public const byte FRAME_TYPE_ANS = 0x01;

        /// <summary>
        /// 通知帧字节
        /// </summary>
        public const byte FRAME_TYPE_INFO = 0x02;


        /// <summary>
        /// 模块信息指令代码
        /// </summary>
        public const byte FRAME_CMD_MODULE_INFO = 0x03;

        /// <summary>
        /// 单独EPC指令代码
        /// </summary>
        public const byte FRAME_CMD_READ_SINGLE = 0x22;

        /// <summary>
        /// 多读EPC指令代码
        /// </summary>
        public const byte FRAME_CMD_READ_MULTI = 0x27;

        /// <summary>
        /// 停止多次轮询操作的指令代码
        /// </summary>
        public const byte FRAME_CMD_STOP_READ = 0x28;

        /// <summary>
        /// 设置读卡器工作区的指令代码
        /// </summary>
        public const byte FRAME_CMD_SET_REGION = 0x07;

        /// <summary>
        /// 获取读卡器发射功率的指令代码
        /// </summary>
        public const byte FRAME_CMD_GET_PA_POWER = (byte)0xB7;

        /// <summary>
        /// 设置读卡器发射功率的指令代码
        /// </summary>
        public const byte FRAME_CMD_SET_PA_POWER = (byte)0xB6;

        /// <summary>
        /// 设置Select参数的指令代码
        /// </summary>
        public const byte FRAME_CMD_SET_SELECT = 0x0C;

        /// <summary>
        /// Kill标签指令代码
        /// </summary>
        public const byte FRAME_CMD_KILL = 0x65;

        /// <summary>
        /// Lock标签指令代码
        /// </summary>
        public const byte FRAME_CMD_LOCK = (byte)0x82;

        /// <summary>
        /// 读取标签数据指令代码
        /// </summary>
        public const byte FRAME_CMD_READ_DATA = 0x39;

        /// <summary>
        /// 写标签数据指令代码
        /// </summary>
        public const byte FRAME_CMD_WRITE_DATA = 0x49;

        /// <summary>
        /// 设置工作信道指令代码
        /// </summary>
        public const byte FRAME_CMD_SET_CHANNEL = (byte)0xAB;

        /// <summary>
        /// 获取工作信道指令代码
        /// </summary>
        public const byte FRAME_CMD_GET_CHANNEL = (byte)0xAA;

        /// <summary>
        /// 获取固件中Query指令代码
        /// </summary>
        public const byte FRAME_CMD_GET_QUERY = 0x0D;

        /// <summary>
        /// 设置固件Query指令代码
        /// </summary>
        public const byte FRAME_CMD_SET_QUERY = 0x0E;

        /// <summary>
        /// 设置自动跳频指令代码
        /// </summary>
        public const byte FRAME_CMD_SET_FHSS = (byte)0xAD;

        /// <summary>
        /// 设置Select模式指令代码
        /// </summary>
        public const byte FRAME_CMD_SET_SELECT_MODEL = 0x12;

        /* 指令参数 */

        /* ----- 模块信息 ----- */
        /// <summary>
        /// 固件版本指令参数
        /// </summary>
        public const byte FRAME_PARAM_FIRMWARE_VERSION = 0x00;

        /// <summary>
        /// 软件版本指令参数
        /// </summary>
        public const byte FRAME_PARAM_SOFTWARE_VERSION = 0x01;

        /// <summary>
        /// 制造商信息指令参数
        /// </summary>
        public const byte FRAME_PARAM_MANUFACTURERS_INFO = 0x02;

        /* ----- 其他 ----- */

        /// <summary>
        /// 保留位指令参数
        /// </summary>
        public const byte FRAME_PARAM_RESERVED = 0x22;

        /* ----- 区域代码参数 ----- */

        /// <summary>
        /// 中国900MHz地区代码指令参数
        /// </summary>
        public const byte FRAME_PARAM_REGION_CHINA_900_MHZ = 0x01;

        /// <summary>
        /// 中国800MHz地区代码指令参数
        /// </summary>
        public const byte FRAME_PARAM_REGION_CHINA_800_MHZ = 0x04;

        /// <summary>
        /// 美国地区代码指令参数
        /// </summary>
        public const byte FRAME_PARAM_REGION_USA = 0x02;

        /// <summary>
        /// 欧洲地区代码指令参数
        /// </summary>
        public const byte FRAME_PARAM_REGION_EUROPE = 0x03;

        /// <summary>
        /// 韩国地区代码指令参数
        /// </summary>
        public const byte FRAME_PARAM_REGION_KOREA = 0x06;

        /// <summary>
        /// 标签RFU数据存储区指令参数
        /// </summary>
        public const byte FRAME_PARAM_MEM_BANK_RFU = 0x00;

        /// <summary>
        /// 标签EPC数据存储区指令参数
        /// </summary>
        public const byte FRAME_PARAM_MEM_BANK_EPC = 0x01;

        /// <summary>
        /// 标签TID数据存储区指令参数
        /// </summary>
        public const byte FRAME_PARAM_MEM_BANK_TID = 0x02;

        /// <summary>
        /// 标签USER数据存储区指令参数
        /// </summary>
        public const byte FRAME_PARAM_MEM_BANK_USER = 0x03;

        /// <summary>
        /// 关闭truncate指令参数
        /// </summary>
        public const byte FRAME_PARAM_TRUNCATE_DISABLE = 0x00;

        /// <summary>
        /// 开启truncate指令参数
        /// </summary>
        public const byte FRAME_PARAM_TRUNCATE_ENABLE = (byte)0x80;
    }

    public class Commands
    {
        /// <summary>
        /// 构建一个完整的数据帧
        /// </summary>
        /// <param name="data">内容区(一个完整帧去除掉开始标志位、结束标志位和校验位)</param>
        /// <returns>完整数据帧</returns>
        public static byte[] BuildFrame(params byte[] data)
        {
            byte[] result = new byte[data.Length + 3];
            result[0] = ConstCode.FRAME_BEGIN;
            Array.Copy(data, 0, result, 1, data.Length);
            result[result.Length - 2] = CalculateCheckSum(data);
            result[result.Length - 1] = ConstCode.FRAME_END;
            return result;
        }

        /// <summary>
        /// 构建一个完整的数据帧
        /// </summary>
        /// <param name="fType">帧类型</param>
        /// <param name="cmdCode">指令代码</param>
        /// <param name="data">指定参数</param>
        /// <returns>完整数据帧</returns>
        public static byte[] BuildFrame(byte fType, byte cmdCode, params byte[] data)
        {
            byte[] content = new byte[data.Length + 4];
            content[0] = fType;
            content[1] = cmdCode;
            content[2] = (byte)((data.Length >> 8) & 0xFF);
            content[3] = (byte)(data.Length & 0xFF);
            Array.Copy(data, 0, content, 4, data.Length);
            return BuildFrame(content);
        }

        /// <summary>
        /// 构建一个帧类型为命令帧的完整数据帧
        /// </summary>
        /// <param name="cmdCode">指令代码</param>
        /// <param name="data">指定参数</param>
        /// <returns>命令帧</returns>
        public static byte[] BuildCMDFrame(byte cmdCode, params byte[] data)
        {
            return BuildFrame(ConstCode.FRAME_TYPE_CMD, cmdCode, data);
        }

        /// <summary>
        /// 构建一个用于获取固件版本的命令帧
        /// </summary>
        /// <returns>命令帧</returns>
        public static byte[] BuildGetFirmwareVersionFrame()
        {
            return BuildCMDFrame(ConstCode.FRAME_CMD_MODULE_INFO, ConstCode.FRAME_PARAM_FIRMWARE_VERSION);
        }

        /// <summary>
        /// 构建一个用于获取软件版本的命令帧
        /// </summary>
        /// <returns>命令帧</returns>
        public static byte[] BuildGetSoftwareVersionFrame()
        {
            return BuildCMDFrame(ConstCode.FRAME_CMD_MODULE_INFO, ConstCode.FRAME_PARAM_SOFTWARE_VERSION);
        }

        /// <summary>
        /// 构建一个用于获取制造商信息的命令帧
        /// </summary>
        /// <returns>命令帧</returns>
        public static byte[] BuildGetManufacturersInfoFrame()
        {
            return BuildCMDFrame(ConstCode.FRAME_CMD_MODULE_INFO, ConstCode.FRAME_PARAM_MANUFACTURERS_INFO);
        }

        /// <summary>
        /// 构建一个单读EPC的命令帧
        /// </summary>
        /// <returns>命令帧</returns>
        public static byte[] BuildReadSingleFrame()
        {
            return BuildCMDFrame(ConstCode.FRAME_CMD_READ_SINGLE);
        }

        /// <summary>
        /// 构建一个多读EPC的命令帧
        /// </summary>
        /// <param name="loopNum">轮询次数</param>
        /// <returns>命令帧</returns>
        public static byte[] BuildReadMultiFrame(int loopNum)
        {
            if (loopNum <= 0 || loopNum > 65535) return new byte[0];
            return BuildCMDFrame(
                    ConstCode.FRAME_CMD_READ_MULTI,
                    ConstCode.FRAME_PARAM_RESERVED,
                    (byte)((loopNum >> 8) & 0xFF),
                    (byte)(loopNum & 0xFF));
        }

        /// <summary>
        /// 构建一个立即停止多次轮询操作的命令帧
        /// </summary>
        /// <returns>命令帧</returns>
        public static byte[] BuildStopReadFrame()
        {
            return BuildCMDFrame(ConstCode.FRAME_CMD_STOP_READ);
        }

        /// <summary>
        /// 构建一个设置读卡器工作区的命令帧
        /// </summary>
        /// <param name="regionCode">地区代码，目前支持：中国900MHz -> 0x01；中国800MHz -> 0x04；美国 -> 0x02；欧洲 -> 0x03；韩国 -> 0x06</param>
        /// <returns>命令帧</returns>
        public static byte[] BuildSetRegionFrame(byte regionCode)
        {
            return BuildCMDFrame(ConstCode.FRAME_CMD_SET_REGION, regionCode);
        }

        /// <summary>
        /// 构建一个获取读卡器发射功率的命令帧
        /// </summary>
        /// <returns>命令帧</returns>
        public static byte[] BuildGetPaPowerFrame()
        {
            return BuildCMDFrame(ConstCode.FRAME_CMD_GET_PA_POWER);
        }

        /// <summary>
        /// 构建一个设置读卡器发射功率的命令帧
        /// </summary>
        /// <param name="powerdBm">发射功率</param>
        /// <returns>命令帧</returns>
        public static byte[] BuildSetPaPowerFrame(int powerdBm)
        {
            if (powerdBm < 0) return new byte[0];
            return BuildCMDFrame(
                    ConstCode.FRAME_CMD_SET_PA_POWER,
                    (byte)((powerdBm >> 8) & 0xFF),
                    (byte)(powerdBm & 0xFF));
        }

        /// <summary>
        /// 构建一个设置Select参数的命令帧
        /// </summary>
        /// <param name="target"></param>
        /// <param name="action"></param>
        /// <param name="memBank">标签数据存储区</param>
        /// <param name="pointer"></param>
        /// <param name="truncated">是否truncated</param>
        /// <param name="mask">mask内容</param>
        /// <returns>命令帧</returns>
        public static byte[] BuildSetSelectFrame(byte target, byte action, byte memBank, long pointer, bool truncated, byte[] mask)
        {
            byte selParam = (byte)((target << 5 | ((action & 0x07) << 2) | (memBank & 0x03)) & 0xFF);
            byte lenByte = (byte)((mask.Length * 8) & 0xFF);

            byte[] data = new byte[7 + mask.Length];
            data[0] = selParam;
            data[1] = (byte)((pointer >> 24) & 0xFF);
            data[2] = (byte)((pointer >> 16) & 0xFF);
            data[3] = (byte)((pointer >> 8) & 0xFF);
            data[4] = (byte)(pointer & 0xFF);
            data[5] = lenByte;
            data[6] = truncated ? ConstCode.FRAME_PARAM_TRUNCATE_ENABLE : ConstCode.FRAME_PARAM_TRUNCATE_DISABLE;
            Array.Copy(mask, 0, data, 7, mask.Length);

            return BuildCMDFrame(ConstCode.FRAME_CMD_SET_SELECT, data);
        }

        /// <summary>
        /// 构建一个灭活标签的命令帧
        /// </summary>
        /// <param name="password">kill密码</param>
        /// <returns>命令帧</returns>
        public static byte[] BuildKillFrame(byte[] password)
        {
            return BuildCMDFrame(ConstCode.FRAME_CMD_KILL, password);
        }

        /// <summary>
        /// 构建一个锁定或解锁标签的命令帧
        /// </summary>
        /// <param name="accessPassword">access密码</param>
        /// <param name="ld">Lock操作数，详细含义请参见EPC Gen2协议1.2.0版6.3.2.11.3.5节</param>
        /// <returns>命令帧</returns>
        public static byte[] BuildLockFrame(byte[] accessPassword, int ld)
        {
            int acccessPwdLength = accessPassword.Length;
            byte[] dataBytes = new byte[acccessPwdLength + 3];
            Array.Copy(accessPassword, 0, dataBytes, 0, acccessPwdLength);
            dataBytes[acccessPwdLength++] = (byte)((ld >> 16) & 0xFF);
            dataBytes[acccessPwdLength++] = (byte)((ld >> 8) & 0xFF);
            dataBytes[acccessPwdLength++] = (byte)(ld & 0xFF);
            return BuildCMDFrame(ConstCode.FRAME_CMD_LOCK, dataBytes);
        }

        /// <summary>
        /// 构建一个读取标签数据的命令帧
        /// </summary>
        /// <param name="accessPassword">access密码</param>
        /// <param name="memBank">标签数据区</param>
        /// <param name="sa">读标签数据区地址偏移</param>
        /// <param name="dl">读标签数据区地址长度</param>
        /// <returns>命令帧</returns>
        public static byte[] BuildReadDataFrame(byte[] accessPassword, byte memBank, int sa, int dl)
        {
            int accessPwdL = accessPassword.Length;
            byte[] dataBytes = new byte[accessPwdL + 5];
            Array.Copy(accessPassword, 0, dataBytes, 0, accessPwdL);
            dataBytes[accessPwdL++] = memBank;
            dataBytes[accessPwdL++] = (byte)((sa >> 8) & 0xFF);
            dataBytes[accessPwdL++] = (byte)(sa & 0xFF);
            dataBytes[accessPwdL++] = (byte)((dl >> 8) & 0xFF);
            dataBytes[accessPwdL++] = (byte)(dl & 0xFF);
            return BuildCMDFrame(ConstCode.FRAME_CMD_READ_DATA, dataBytes);
        }

        /// <summary>
        /// 构建一个写入标签数据的命令帧
        /// </summary>
        /// <param name="accessPassword">access密码</param>
        /// <param name="memBank">标签数据区</param>
        /// <param name="sa">标签数据区地址偏移</param>
        /// <param name="dl">标签数据区地址长度</param>
        /// <param name="dt">写入数据</param>
        /// <returns>命令帧</returns>
        public static byte[] BuildWriteDataFrame(byte[] accessPassword, byte memBank, int sa, int dl, byte[] dt)
        {
            int accessPwdL = accessPassword.Length;
            int dtL = dt.Length;
            byte[] dataBytes = new byte[accessPwdL + dtL + 5];
            Array.Copy(accessPassword, 0, dataBytes, 0, accessPwdL);
            dataBytes[accessPwdL++] = memBank;
            dataBytes[accessPwdL++] = (byte)((sa >> 8) & 0xFF);
            dataBytes[accessPwdL++] = (byte)(sa & 0xFF);
            dataBytes[accessPwdL++] = (byte)((dl >> 8) & 0xFF);
            dataBytes[accessPwdL++] = (byte)(dl & 0xFF);
            Array.Copy(dt, 0, dataBytes, accessPwdL, dtL);
            return BuildCMDFrame(ConstCode.FRAME_CMD_WRITE_DATA, dataBytes);
        }

        /// <summary>
        /// 构建一个设置工作信道的命令帧
        /// </summary>
        /// <param name="ch">信道代码</param>
        /// <returns>命令帧</returns>
        public static byte[] BuildSetChannelFrame(byte ch)
        {
            return BuildCMDFrame(ConstCode.FRAME_CMD_SET_CHANNEL, ch);
        }

        /// <summary>
        /// 构建一个获取工作信道的命令帧
        /// </summary>
        /// <returns>命令帧</returns>
        public static byte[] BuildGetChannelFrame()
        {
            return BuildCMDFrame(ConstCode.FRAME_CMD_GET_CHANNEL);
        }

        /// <summary>
        /// 构建一个获取固件Query值的命令帧
        /// </summary>
        /// <returns>命令帧</returns>
        public static byte[] BuildGetQueryFrame()
        {
            return BuildCMDFrame(ConstCode.FRAME_CMD_GET_QUERY);
        }

        /// <summary>
        /// 构建一个设置固件Query值的命令帧
        /// </summary>
        /// <param name="dr"></param>
        /// <param name="m"></param>
        /// <param name="tREXT"></param>
        /// <param name="sel"></param>
        /// <param name="session"></param>
        /// <param name="targer"></param>
        /// <param name="q">Q值</param>
        /// <returns>命令帧</returns>
        public static byte[] BuildSetQueryFrame(int dr, int m, int tREXT, int sel, int session, int targer, int q)
        {
            byte msb = (byte)(dr << 7 | m << 5 | tREXT << 4 | sel << 2 | session);
            byte lsb = (byte)(targer << 7 | q << 3);
            return BuildCMDFrame(ConstCode.FRAME_CMD_SET_QUERY, msb, lsb);
        }

        /// <summary>
        /// 构建一个设置固件为自动跳频模式或者取消自动跳频模式的命令帧
        /// </summary>
        /// <param name="isAutoFhss">true：自动跳频; false:取消自动跳频</param>
        /// <returns>命令帧</returns>
        public static byte[] BuildSetFhssFrame(bool isAutoFhss)
        {
            return BuildCMDFrame(ConstCode.FRAME_CMD_SET_FHSS, isAutoFhss ? (byte)0xFF : (byte)0x00);
        }

        /// <summary>
        /// 构建一个设置Select模式的命令帧
        /// </summary>
        /// <param name="selectModel">Select模式</param>
        /// <returns>命令帧</returns>
        public static byte[] BuildSetSelectModelFrame(byte selectModel)
        {
            return BuildCMDFrame(ConstCode.FRAME_CMD_SET_SELECT_MODEL, selectModel);
        }

        /// <summary>
        /// 计算校验位
        /// </summary>
        private static byte CalculateCheckSum(params byte[] data)
        {
            int checksum = 0;
            foreach (byte item in data)
            {
                checksum += (item & 0xFF);
            }
            checksum %= 256;
            return (byte)checksum;
        }
    }
}
