-----------------------------------------------------------------------
-- 使用说明：                                                        --
-- V1.0                                                              --
-- 用户需要自行完成一下2个函数的实现                                 --
-- 1、定时下发数据任务初始化函数：device_timer_init(dev)【可选】     --
-- 2、对设备上传数据进行解析（包括心跳等）：device_data_analyze(dev) --
-----------------------------------------------------------------------


-------------------------------------------------------
-- 注册C函数                                         --
-- u2f 将32位整型内存数据转换为浮点数（不同于值转换）--
-- 类似C代码 *(float*)(&u)                           --
-- function utf(u)                                   --
-- @param   u   number   整数值                      --
-- @return  成功返回浮点数值，否则返回nil            --
-- @example local u = 123                            --
--          local f = u2f( 123 )                     --
-------------------------------------------------------

--------------------------------------------------------
-- 将bytes string转换hex string                       --
-- @param   s   string   bytes string                 --
-- @return  返回hex string，类似"0A0B0C0D..."         --
-- @example local hex = to_hex("\2\2\0\150\0\37\206") --
--------------------------------------------------------
function to_hex(s)
	local i
	local t

	t={s:byte(1,s:len())}
	for i=1,#t do
		t[i]=string.format('%02X',t[i])
	end

	return table.concat(t)
end

-----------------------------------------------
-- 将object序列化成字符串                    --
-- @param   o   boolean|number|string|table  --
-- @return  返回序列化string                 --
-- @example local str = to_str({x=100})      --
-----------------------------------------------
function to_str(o)
	local i=1
	local t={}
	local f

	f=function(x)
		local y=type(x)
		if y=="number" then
			t[i]=x
			i=i+1
		elseif y=="boolean" then
			t[i]=tostring(x)
			i=i+1
		elseif y=="string" then
			t[i]="\""
			t[i+1]=x
			t[i+2]="\""
			i=i+3
		elseif y=="table" then
			t[i]="{"
			i=i+1

			local z=true
			for k,v in pairs(x) do
				if z then
					z=false
					t[i]="\""
					t[i+1]=k
					t[i+2]="\""
					t[i+3]=":"
					i=i+4
					f(v)
				else
					t[i]=","
					t[i+1]="\""
					t[i+2]=k
					t[i+3]="\""
					t[i+4]=":"
					i=i+5
					f(v)
				end
			end

			t[i]="}"
			i=i+1
		else
			t[i]="nil"
			i=i+1
		end
	end
	f(o)

	return table.concat(t)
end

----------------------------------------------------------------------------------------------------------
-- 添加值数据点到table中                                                                                --
-- @param   t   table                                                                                   --
--          i   string                      数据流或数据流模板名称                                      --
--          a   number                      毫秒级时间戳，距离（00:00:00 UTC, January 1, 1970）的毫秒； --
--                                          如果值为0，表示使用当前时间                                 --
--          v   boolean|number|string|table 布尔值、数值、字符串、json                                  --
-- @return  成功返回true，否则返回false                                                                 --
-- @example local ok = add_val(t,"dsname",0,100)                                                        --
----------------------------------------------------------------------------------------------------------
function add_val(t, i, a, v)
	if type(t)~="table" then
		return false
	elseif type(i)~="string" then
		return false
	elseif type(a)~="number" then
		return false
	else
		local o = type(v)
		if o~="boolean" and o~="number" and o~="string" and o~="table" then
			return false
		end

		local n = {i=i,v=v}
		if a~=0 then
			n["a"]=a
		end

		-- list push_back --
		if t.h==nil then
			t.h={nil,n}
			t.t=t.h
		else
			t.t[1]={nil,n}
			t.t=t.t[1]
		end
	end

	return true
end

----------------------------------------------------------------------------------------------------------
-- 添加二进制数据点到table中                                                                            --
-- @param   t   table                                                                                   --
--          i   string                      数据流或数据流模板名称                                      --
--          a   number                      毫秒级时间戳，距离（00:00:00 UTC, January 1, 1970）的毫秒； --
--                                          如果值为0，表示使用当前时间                                 --
--          b   string                      二进制数据（hex string），类似"0A0B0C0D..."                 --
--          d   boolean|number|string|table 用于描述b（可选）；数值、字符串、json                       --
-- @return  成功返回true，否则返回false                                                                 --
-- @example local ok = add_val(t,"dsname",0,"0A0B0C0D...",{...})                                        --
----------------------------------------------------------------------------------------------------------
function add_bin(t, i, a, b, d)
	if type(t)~="table" then
		return false
	elseif type(i)~="string" then
		return false
	elseif type(a)~="number" then
		return false
	elseif type(b)~="string" then
		return false
	else
		local o=type(d)
		if o~="nil" and o~="string" and o~="table" then
			return false
		end

		local n={i=i,b=to_hex(b)}
		if a~=0 then
			n["a"]=a
		end
		if d~=nil then
			n["d"]=d
		end

		-- list push_back --
		if t.h==nil then
			t.h={nil,n}
			t.t=t.h
		else
			t.t[1]={nil,n}
			t.t=t.t[1]
		end
	end

	return true
end

--------------------------------------------------------------
-- 将table序列化成json字符串                                --
-- @param   t   table   通过add_val、add_bin构建起来的table --
-- @return  返回序列化json字符串                            --
-- @example local json = to_json(t)                         --
--------------------------------------------------------------
function to_json(t)
	local i=1
	local o={}
	local n

	o[i]="["
	i=i+1
	n=t.h
	while n~=nil do
		if n[2]~=nil then
			o[i]=to_str(n[2])
			i=i+1
		end

		n=n[1]
		if n~=nil then
			o[i]=","
			i=i+1
		end
	end
	o[i]="]"

	return table.concat(o)
end

------------------------------------
-- begin-添加用户自定义值或函数等 --

-- end-添加用户自定义值或函数等   --
------------------------------------

------------------------------------------------------------------------------------------
-- 设置定时下发设备的数据（可选）                                             --
-- @param  dev    user_data   设备管理器                                                --
-- @return 无                                                                           --
-- @notice 此函数为回调函数，不可在脚本内调用                                           --
-- @readme dev提供一下几个函数：                                                        --
--         dev:add(interval,name,data)添加定时下发数据                                  --
--           @param   interval   number   数据下发的时间间隔（秒）                      --
--                    name       string   名称（须保证唯一性）                          --
--                    data       string   数据（二进制数据），使用lua转义字符串         --
--           @return  成功返回true，否则返回false                                       --
--           @notice  定时数据下发的平均频率不超过1，及1/interval_1+...+1/interval_n<=1 --
--           @example local ok = dev:add(10,”test”,”\1\1\0\150\0\37\253\29”)        --
--         dev:timeout(sec)设置下发数据的设备响应超时时间（秒）                         --
--           @param   sec        int      响应超时时间（秒）                            --
--           @return  无                                                                --
--           @example dev:timeout(3)                                                    --
--         dev:response()设备响应成功                                                   --
--           @param   无                                                                --
--           @return  无                                                                --
--           @example dev:response()                                                    --
--         dev:send(data)下发数据到设备                                                 --
--           @param   data   string   数据（二进制数据），使用lua转义字符串             --
--           @return  无                                                                --
--           @example dev:send("\2\2\0\150\0\37\206\89")                                --
------------------------------------------------------------------------------------------
function device_timer_init(dev)
	-- 添加用户自定义代码 --
	-- 例如： --
	dev:timeout(3)
	--dev:add(10,"heart beat","heart beat\r\n")
end

-----------------------------------------------------------------------------------------------------------
-- 解析设备上传数据                                                                                      --
-- @param  dev    user_data   设备管理器                                                                 --
-- @return size表示已解析设备上传数据的字节数，json表示解析后的数据点集合，格式如下：                    --
--         [                                                                                             --
--           {                                                                                           --
--             “i” : ”dsname1”,         // 数据流或数据流模板名称1                                   --
--             “a” : 1234567890,          // 毫秒级时间戳，距离（00:00:00 UTC, January 1, 1970）的毫秒 --
--                                          // 如果值为0，表示使用当前时间                               --
--             “v” : 123 | ”123” | {…} // 布尔值、数值、字符串、json                                --
--             “b” : “0A0B0C0D…”       // 二进制数据（16进制字符串），与v互斥，不同时存在           --
--             “d” : xxx | “xxx” | {…} // 用于描述b（可选）；布尔值、数值、字符串、json             --
--           }                                                                                           --
--           …                                                                                          --
--           {                                                                                           --
--             “i” : ”dsnamen”,         // 数据流或数据流模板名称n                                   --
--             “a” : 1234567890,          // 毫秒级时间戳，距离（00:00:00 UTC, January 1, 1970）的毫秒 --
--                                          // 如果值为0，表示使用当前时间                               --
--             “v” : 123 | ”123” | {…} // 布尔值、数值、字符串、json                                --
--             “b” : “0A0B0C0D…”       // 二进制数据（16进制字符串），与v互斥，不同时存在           --
--             “d” : xxx | “xxx” | {…} // 用于描述b（可选）；布尔值、数值、字符串、json             --
--           }                                                                                           --
--         ]                                                                                             --
-- @notice 此函数为回调函数，不可在脚本内调用                                                            --
-- @readme dev提供一下几个函数：                                                                         --
--         dev:add(interval,name,data)添加定时下发数据                                                   --
--           @param   interval number   数据下发的时间间隔（秒）                                       --
--                    name     string   名称（须保证唯一性）                                           --
--                    data     string   数据（二进制数据），使用lua转义字符串                          --
--           @return  成功返回true，否则返回false                                                        --
--           @notice  定时数据下发的平均频率不超过1，及1/interval_1+...+1/interval_n<=1                  --
--           @example local ok = dev:add(10,”test”,”\1\1\0\150\0\37\253\29”)                         --
--         dev:timeout(sec)设置下发数据的设备响应超时时间（秒）                                          --
--           @param   sec      int      响应超时时间（秒）                                             --
--           @return  无                                                                                 --
--           @example dev:timeout(3)                                                                     --
--         dev:response()设备响应成功                                                                    --
--           @param   无                                                                                 --
--           @return  无                                                                                 --
--           @example dev:response()                                                                     --
--         dev:send(data)下发数据到设备                                                                  --
--           @param   data   string   数据（二进制数据），使用lua转义字符串                              --
--           @return  无                                                                                 --
--           @example dev:send("\2\2\0\150\0\37\206\89")                                                 --
--         dev:size()获取设备数据大小（字节数）                                                          --
--           @param   无                                                                                 --
--           @return  返回设备数据大小（字节数）                                                         --
--           @example local sz = dev:size()                                                              --
--         dev:byte(pos)获取pos对应位置的设备数据（字节）                                                --
--           @param   pos   number   指定的获取位置，取值范围[1,dev:size()+1)                            --
--           @return  成功返回设备数据（int），否则返回nil                                               --
--           @example local data = dev:byte(1)                                                           --
--         dev:bytes(pos,count)获取从pos开始，count个设备数据                                            --
--           @param   pos   number   指定的获取起始位置，取值范围[1,dev:size()+1)                        --
--                    count number   指定的获取数据总数，取值范围[0,dev:size()+1-pos]                    --
--           @return  成功返回设备数据（string），否则返回nil                                            --
--           @example local datas = dev:bytes(1,dev:size())                                              --
-----------------------------------------------------------------------------------------------------------
function device_data_analyze(dev)
	local t={}
	local a=0

	-- 添加用户自定义代码 --
	-- 例如： --
	local s = dev:size()
	add_bin(t,"ds_test",a,dev:bytes(1,s) )
	dev:response()
	dev:send("received")

	-- return $1,$2 --
	-- 例如： --
	return s,to_json(t)
end