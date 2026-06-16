using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;
using System.IO;

namespace firmwareupdate
{
    public enum InitialCrcValue { Zeros, NonZero1 = 0xffff, NonZero2 = 0x1D0F }

    public class Crc16Ccitt
    {
        const ushort poly = 4129;
        ushort[] table = new ushort[256];
        ushort initialValue = 0;

        public ushort ComputeChecksum(byte[] bytes)
        {
            ushort crc = this.initialValue;
            for (int i = 0; i < bytes.Length; i++)
            {
                crc = (ushort)((crc << 8) ^ table[((crc >> 8) ^ (0xff & bytes[i]))]);
            }
            return crc;
        }

        public byte[] ComputeChecksumBytes(byte[] bytes)
        {
            ushort crc = ComputeChecksum(bytes);
            return new byte[] { (byte)(crc >> 8), (byte)(crc & 0x00ff) };
        }

        public Crc16Ccitt(InitialCrcValue initialValue)
        {
            this.initialValue = (ushort)initialValue;
            ushort temp, a;
            for (int i = 0; i < table.Length; i++)
            {
                temp = 0;
                a = (ushort)(i << 8);
                for (int j = 0; j < 8; j++)
                {
                    if (((temp ^ a) & 0x8000) != 0)
                    {
                        temp = (ushort)((temp << 1) ^ poly);
                    }
                    else
                    {
                        temp <<= 1;
                    }
                    a <<= 1;
                }
                table[i] = temp;
            }
        }
    }

    class Program
    {
        static SerialPort _serialPort;
        static byte[] firmware_image;

        static bool load_firmware(string filename)
        {
            Console.WriteLine("loading");
            firmware_image = File.ReadAllBytes(filename);

            if (firmware_image.Length > 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        static bool transfer_firmware()
        {
            int arr_index = 0;
            UInt16 packet_number = 0;
            int firmware_image_increment = 128;
            int firmware_image_left = 0;
            byte[] packet;

            Console.WriteLine("Initializing download process");
            Console.WriteLine("Erase flash header");
            // issue a flash erase to the device
            packet = new byte[7]; // [ADDRESS MSB][ADDRESS LSB][MAIN MSB][MAIN LSB][SPEC MSVB][SPEC LSB][ACCESS][CRC16 MSB][CRC16 LSB]
            packet[0] = 0xFF;
            packet[1] = 0xFF;
            packet[2] = 0x00; // firmware commands
            packet[3] = 0xAA; // firmware commands
            packet[4] = 0x00; // specific command 0x00 (erase firmare header section)
            packet[5] = 0x03; // specific command 0x03 (erase firmare header section)
            packet[6] = 0x01; // write
            //  - calculate crc16
            Crc16Ccitt crc16 = new Crc16Ccitt(InitialCrcValue.Zeros);
            byte[] packet_crc = crc16.ComputeChecksumBytes(packet);
            //  - send packet
            byte[] tx_packet = new byte[packet.Length + packet_crc.Length];
            Array.Copy(packet, 0, tx_packet, 0, packet.Length);
            Array.Copy(packet_crc, 0, tx_packet, packet.Length, packet_crc.Length);
            _serialPort.DiscardOutBuffer();
            _serialPort.DiscardInBuffer();
            _serialPort.Write(tx_packet, 0, tx_packet.Length);
            //  - wait for response back [ACK][PACKET NUMBER MSB][PACKET NUMBER LSB]            
            while (_serialPort.BytesToRead != 9)
            {
                System.Threading.Thread.Sleep(10);
            }; // expected response is 9 bytes
            byte[] myread = new byte[_serialPort.BytesToRead];
            _serialPort.Read(myread, 0, _serialPort.BytesToRead);
            // check if erase was acknowledged
            if (myread[0] != 0x06)
            {
                Console.WriteLine("Did not get ACK from download initialization");
                return false; // erase did not get acknowledged exit the application
            }
            else
            {
                Console.WriteLine("initialization complete");
                Console.Write("device resetting");
            }
            
            // issue a device reset
            packet = new byte[8]; // [ADDRESS MSB][ADDRESS LSB][MAIN MSB][MAIN LSB][SPEC MSVB][SPEC LSB][ACCESS][CRC16 MSB][CRC16 LSB]
            packet[0] = 0xFF;
            packet[1] = 0xFF;
            packet[2] = 0x00; // firmware commands
            packet[3] = 0xAA; // firmware commands
            packet[4] = 0x00; // specific command 0x00 (reset)
            packet[5] = 0x05; // specific command 0x05 (reset)
            packet[6] = 0x01; // write
            packet[7] = 0x03; // reset device now!
            //  - calculate crc16
            crc16 = new Crc16Ccitt(InitialCrcValue.Zeros);
            packet_crc = crc16.ComputeChecksumBytes(packet);
            //  - send packet
            tx_packet = new byte[packet.Length + packet_crc.Length];
            Array.Copy(packet, 0, tx_packet, 0, packet.Length);
            Array.Copy(packet_crc, 0, tx_packet, packet.Length, packet_crc.Length);
            _serialPort.DiscardOutBuffer();
            _serialPort.DiscardInBuffer();
            _serialPort.Write(tx_packet, 0, tx_packet.Length);

            // waiting for reset to execute
            //_serialPort.Close();
            int tryCount = 0;
            while (tryCount < 1000)
            {
                try
                {
                    System.Threading.Thread.Sleep(10);
                    _serialPort.Close();
                    break;
                }
                catch (Exception)
                {
                    tryCount++;
                    continue;
                }
            }
            for (int wait = 0; wait < 6; wait++)
            {
                Console.Write(".");
                System.Threading.Thread.Sleep(500); // wait for device to bootup
            }
            Console.WriteLine(".");
            _serialPort.Open();
            System.Threading.Thread.Sleep(500);

            // transfer the image
            while (arr_index < firmware_image.Length)
            {

                // transfer packet_increment at a time
                firmware_image_left = firmware_image.Length - arr_index;
                if (firmware_image_left > firmware_image_increment)
                {
                    packet = new byte[9 + firmware_image_increment]; // [ADDRESS MSB][ADDRESS LSB][COMMAND MSB][COMMAND LSB][ACCESS][PACKET NUMBER MSB][PACKET NUMBER LSB][0]..[1023][CRC16 MSB][CRC16 LSB]
                    Array.Copy(firmware_image, arr_index, packet, 9, firmware_image_increment);
                }
                else
                {
                    packet = new byte[9 + firmware_image_left]; // [ADDRESS MSB][ADDRESS LSB][COMMAND MSB][COMMAND LSB][ACCESS][PACKET NUMBER MSB][PACKET NUMBER LSB][0]..[1023][CRC16 MSB][CRC16 LSB]

                    firmware_image_increment = firmware_image_left;
                    Array.Copy(firmware_image, arr_index, packet, 9, firmware_image_left);
                }

                // assemble the packet 
                packet[0] = 0xFF;
                packet[1] = 0xFF;
                packet[2] = 0x00; // firmware command
                packet[3] = 0xAA; // firmware command
                packet[4] = 0x00; // specific command
                packet[5] = 0x01; // specific command
                packet[6] = 0x01; // write
                packet[7] = (byte)(packet_number >> 8);
                packet[8] = (byte)(packet_number & 0x00ff);

                //  - calculate crc16
                crc16 = new Crc16Ccitt(InitialCrcValue.Zeros);
                packet_crc = crc16.ComputeChecksumBytes(packet);

                //  - send packet
                tx_packet = new byte[packet.Length + packet_crc.Length];
                Array.Copy(packet, 0, tx_packet, 0, packet.Length);
                Array.Copy(packet_crc, 0, tx_packet, packet.Length, packet_crc.Length);
                _serialPort.DiscardOutBuffer();
                _serialPort.DiscardInBuffer();
                _serialPort.Write(tx_packet, 0, tx_packet.Length);                
                //  - wait for response back [ACK][PACKET NUMBER MSB][PACKET NUMBER LSB]
                while (_serialPort.BytesToRead != 9)
                {
                    System.Threading.Thread.Sleep(10);
                }; // expected response is 11 bytes
                myread = new byte[_serialPort.BytesToRead];
                _serialPort.Read(myread, 0, _serialPort.BytesToRead);
                if (myread[0] == 0x06)
                {
                    Console.WriteLine("Packet " + packet_number.ToString() + " processed");
                    arr_index += firmware_image_increment;
                    packet_number++;
                }
                else
                {
                    Console.WriteLine("Packet " + packet_number.ToString() + " error");
                    return false;
                }
            }
            // send a write header command to complete the download
            Console.WriteLine("Finalizing");
            // issue a flash erase to the device
            packet = new byte[7]; // [ADDRESS MSB][ADDRESS LSB][MAIN MSB][MAIN LSB][SPEC MSVB][SPEC LSB][ACCESS][CRC16 MSB][CRC16 LSB]
            packet[0] = 0xFF;
            packet[1] = 0xFF;
            packet[2] = 0x00; // firmware commands
            packet[3] = 0xAA; // firmware commands
            packet[4] = 0x00; // specific command 0x00 (write header section)
            packet[5] = 0x02; // specific command 0x02 (write header section)
            packet[6] = 0x01; // write
            //  - calculate crc16
            crc16 = new Crc16Ccitt(InitialCrcValue.Zeros);
            packet_crc = crc16.ComputeChecksumBytes(packet);
            //  - send packet
            tx_packet = new byte[packet.Length + packet_crc.Length];
            Array.Copy(packet, 0, tx_packet, 0, packet.Length);
            Array.Copy(packet_crc, 0, tx_packet, packet.Length, packet_crc.Length);
            _serialPort.DiscardOutBuffer();
            _serialPort.DiscardInBuffer();
            _serialPort.Write(tx_packet, 0, tx_packet.Length);
            //  - wait for response back [ACK][PACKET NUMBER MSB][PACKET NUMBER LSB]
            while (_serialPort.BytesToRead != 9)
            {
                System.Threading.Thread.Sleep(10);
            }; // expected response is 9 bytes
            myread = new byte[_serialPort.BytesToRead];
            _serialPort.Read(myread, 0, _serialPort.BytesToRead);
            // check if erase was acknowledged
            if (myread[0] != 0x06)
            {
                Console.WriteLine("Did not get ACK from header write");
                return false; // erase did not get acknowledged exit the application
            }
            else
            {
                Console.WriteLine("Download complete");
            }
            // issue a device reset
            packet = new byte[8]; // [ADDRESS MSB][ADDRESS LSB][MAIN MSB][MAIN LSB][SPEC MSVB][SPEC LSB][ACCESS][CRC16 MSB][CRC16 LSB]
            packet[0] = 0xFF;
            packet[1] = 0xFF;
            packet[2] = 0x00; // firmware commands
            packet[3] = 0xAA; // firmware commands
            packet[4] = 0x00; // specific command 0x00 (reset)
            packet[5] = 0x05; // specific command 0x05 (reset)
            packet[6] = 0x01; // write
            packet[7] = 0x03; // reset device now!
            //  - calculate crc16
            crc16 = new Crc16Ccitt(InitialCrcValue.Zeros);
            packet_crc = crc16.ComputeChecksumBytes(packet);
            //  - send packet
            tx_packet = new byte[packet.Length + packet_crc.Length];
            Array.Copy(packet, 0, tx_packet, 0, packet.Length);
            Array.Copy(packet_crc, 0, tx_packet, packet.Length, packet_crc.Length);
            _serialPort.DiscardOutBuffer();
            _serialPort.DiscardInBuffer();
            _serialPort.Write(tx_packet, 0, tx_packet.Length);
            // waiting for reset to execut
            _serialPort.Close();
            for (int wait = 0; wait < 6; wait++)
            {
                Console.Write(".");
                System.Threading.Thread.Sleep(500); // wait for device to bootup
            }
            Console.WriteLine(".");
            _serialPort.Close();
            return true;
        }

        /**
            @brief read data from device
        */
        static UInt16 read_raw(UInt16 main_cmd, UInt16 specific_cmd, ref object dataout)
        {
            byte[] packet = new byte[7]; // [ADDRESS MSB][ADDRESS LSB][MAIN MSB][MAIN LSB][SPEC MSVB][SPEC LSB][ACCESS][CRC16 MSB][CRC16 LSB]
            packet[0] = 0xFF;
            packet[1] = 0xFF;
            packet[2] = (byte)(main_cmd >> 8); // powerctrl commands
            packet[3] = (byte)(main_cmd & 0xff); // powerctrl commands
            packet[4] = (byte)(specific_cmd >> 8); // specific command 0x00 (write header section)
            packet[5] = (byte)(specific_cmd & 0xff); // specific command 0x02 (write header section)
            packet[6] = 0x02; // read
            //  - calculate crc16
            Crc16Ccitt crc16 = new Crc16Ccitt(InitialCrcValue.Zeros);
            byte[] packet_crc = crc16.ComputeChecksumBytes(packet);
            //  - send packet
            byte[] tx_packet = new byte[packet.Length + packet_crc.Length];
            Array.Copy(packet, 0, tx_packet, 0, packet.Length);
            Array.Copy(packet_crc, 0, tx_packet, packet.Length, packet_crc.Length);
            _serialPort.DiscardOutBuffer();
            _serialPort.DiscardInBuffer();
            _serialPort.Write(tx_packet, 0, tx_packet.Length);

            // wait for response
            UInt16 mydelay = 100;// Convert.ToUInt16((packet.Length + packet_crc.Length + expected_size + 7 + 4) * msec_per_byte);
            System.Threading.Thread.Sleep(mydelay);

            byte[] data = new byte[_serialPort.BytesToRead];
            byte[] myread = new byte[_serialPort.BytesToRead];
            _serialPort.Read(myread, 0, _serialPort.BytesToRead);

            // validate response packet crc
            byte[] rx_packet = new byte[myread.Length - 2];
            Array.Copy(myread, 0, rx_packet, 0, rx_packet.Length);
            crc16 = new Crc16Ccitt(InitialCrcValue.Zeros);
            byte[] rx_packet_crc = crc16.ComputeChecksumBytes(rx_packet);
            UInt16 rx_crc = (UInt16)((myread[myread.Length - 2] << 8) | myread[myread.Length - 1]);
            UInt16 calc_rx_crc = (UInt16)((rx_packet_crc[0] << 8) | rx_packet_crc[1]);
            // check if erase was acknowledged
            if (rx_crc != calc_rx_crc)
            {
                Console.WriteLine("bad CRC");
                return 1;
            }
            else if (myread[0] != 0x06)
            {
                Console.WriteLine("NAK from device");
                return 2;
            }
            else
            {
                data = new byte[myread.Length - 9];
                Array.Copy(myread, 7, data, 0, data.Length);
                //dataout = data;

                //float dataout = BitConverter.ToSingle(data, 0);
                //Console.WriteLine(dataout.ToString());

                return 0;
            }
        }

        static void protocol_test()
        {
            

            bool exit_test = false;
            Console.WriteLine("Running protocol test engine:");
            do
            {
                string cmd = Console.ReadLine();                
                string[] splits = cmd.Split(',');
            
                switch (splits[0])
                {
                    case "e":
                    case "E":
                        exit_test = true;
                        break;
                    case "r":
                    case "R":
                        if (splits.Length >= 3)
                        {
                            UInt16 main_cmd = Convert.ToUInt16(splits[1]);
                            UInt16 specific_cmd = Convert.ToUInt16(splits[2]);
                            //read_raw(main_cmd, specific_cmd);
                        }
                        else
                        {
                            Console.WriteLine("invalid!");
                        }
                        break;
                    case "w":
                    case "W":
                        break;
                }
                

            } while (!exit_test);
            Console.WriteLine("Exiting protocol test engine:");
        }

        static void Main(string[] args)
        {
            string comport = args[0];
            string baudrate = args[1];
            string firmware_filename = args[2];
            string protocol_tester = args[3];

            Console.WriteLine("Firmware downloader and Firmware utility application 2.1.0");

            Console.WriteLine("Initializing USBClassLibrary");
            //USBPort = new USBClass();
            //ListOfUSBDeviceProperties = new List<USBClassLibrary.USBClass.DeviceProperties>();
            
            
            Console.WriteLine("Initializing serial port");
            // Create a new SerialPort object with default settings.
            _serialPort = new SerialPort();

            // Allow the user to set the appropriate properties.
            _serialPort.PortName = comport; // SetPortName(_serialPort.PortName);
            _serialPort.BaudRate = Convert.ToInt32(baudrate);//SetPortBaudRate(_serialPort.BaudRate);
            _serialPort.Parity = System.IO.Ports.Parity.None; // SetPortParity(_serialPort.Parity);
            _serialPort.DataBits = 8; // SetPortDataBits(_serialPort.DataBits);
            _serialPort.StopBits = System.IO.Ports.StopBits.One; // SetPortStopBits(_serialPort.StopBits);
            _serialPort.Handshake = 0; // SetPortHandshake(_serialPort.Handshake);

            // Set buffer size
            _serialPort.WriteBufferSize = 2048;

            // Set the read/write timeouts
            //_serialPort.ReadTimeout = 500;
            //_serialPort.WriteTimeout = 500;

            _serialPort.Open();

            if (protocol_tester == "-t")
            {
                protocol_test();
            }
            else
            {

                if (load_firmware(firmware_filename))
                {
                    if (transfer_firmware())
                    {
                        Console.WriteLine("fimrware downloaded successful");
                    }
                    else
                    {
                        Console.WriteLine("fimrware downloaded failed");
                    }
                }
            }

            try
            {
                _serialPort.Close(); // Done with firmware download
            } catch {

            }

        }

        // Display Port values and prompt user to enter a port. 
        public static string SetPortName(string defaultPortName)
        {
            string portName;

            Console.WriteLine("Available Ports:");
            foreach (string s in SerialPort.GetPortNames())
            {
                Console.WriteLine("   {0}", s);
            }

            Console.Write("Enter COM port value (Default: {0}): ", defaultPortName);
            portName = Console.ReadLine();

            if (portName == "" || !(portName.ToLower()).StartsWith("com"))
            {
                portName = defaultPortName;
            }
            return portName;
        }
        // Display BaudRate values and prompt user to enter a value. 
        public static int SetPortBaudRate(int defaultPortBaudRate)
        {
            string baudRate;

            Console.Write("Baud Rate(default:{0}): ", defaultPortBaudRate);
            baudRate = Console.ReadLine();

            if (baudRate == "")
            {
                baudRate = defaultPortBaudRate.ToString();
            }

            return int.Parse(baudRate);
        }

        // Display PortParity values and prompt user to enter a value. 
        public static Parity SetPortParity(Parity defaultPortParity)
        {
            string parity;

            Console.WriteLine("Available Parity options:");
            foreach (string s in Enum.GetNames(typeof(Parity)))
            {
                Console.WriteLine("   {0}", s);
            }

            Console.Write("Enter Parity value (Default: {0}):", defaultPortParity.ToString(), true);
            parity = Console.ReadLine();

            if (parity == "")
            {
                parity = defaultPortParity.ToString();
            }

            return (Parity)Enum.Parse(typeof(Parity), parity, true);
        }
        // Display DataBits values and prompt user to enter a value. 
        public static int SetPortDataBits(int defaultPortDataBits)
        {
            string dataBits;

            Console.Write("Enter DataBits value (Default: {0}): ", defaultPortDataBits);
            dataBits = Console.ReadLine();

            if (dataBits == "")
            {
                dataBits = defaultPortDataBits.ToString();
            }

            return int.Parse(dataBits.ToUpperInvariant());
        }

        // Display StopBits values and prompt user to enter a value. 
        public static StopBits SetPortStopBits(StopBits defaultPortStopBits)
        {
            string stopBits;

            Console.WriteLine("Available StopBits options:");
            foreach (string s in Enum.GetNames(typeof(StopBits)))
            {
                Console.WriteLine("   {0}", s);
            }

            Console.Write("Enter StopBits value (None is not supported and \n" +
             "raises an ArgumentOutOfRangeException. \n (Default: {0}):", defaultPortStopBits.ToString());
            stopBits = Console.ReadLine();

            if (stopBits == "")
            {
                stopBits = defaultPortStopBits.ToString();
            }

            return (StopBits)Enum.Parse(typeof(StopBits), stopBits, true);
        }
        public static Handshake SetPortHandshake(Handshake defaultPortHandshake)
        {
            string handshake;

            Console.WriteLine("Available Handshake options:");
            foreach (string s in Enum.GetNames(typeof(Handshake)))
            {
                Console.WriteLine("   {0}", s);
            }

            Console.Write("End Handshake value (Default: {0}):", defaultPortHandshake.ToString());
            handshake = Console.ReadLine();

            if (handshake == "")
            {
                handshake = defaultPortHandshake.ToString();
            }

            return (Handshake)Enum.Parse(typeof(Handshake), handshake, true);
        }
    }
}
