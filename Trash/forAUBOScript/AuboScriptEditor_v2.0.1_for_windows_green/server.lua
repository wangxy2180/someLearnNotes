-----socket server
local socket = require("socket")

local _M = {}


--function _M.sleep(n)
--	socket.select(nil,nil,n)
--end

function _M.startServer(ip,port)
	_M.server = assert(socket.bind(ip, port, 1024))
	_M.server:settimeout(0)
	print("TcpServer Start " .. ip .. ":" .. port) 
end

local client_tab = {}

local conn_count = 0 

function _M.receiveData()
	
	local  conn = _M.server:accept()

        if conn then
 
        conn_count = conn_count + 1

        client_tab[conn_count] = conn

        print("client:" .. client_tab[conn_count]:getpeername() .. ",successfully connect!") 

    end

      for i, client in pairs(client_tab) do local recvt, sendt, status = socket.select({client}, nil, 0.1)

        if #recvt > 0 then _M.receive, receive_status = client:receive()
  		
            if receive_status ~= "closed" then

                if _M.receive then

                    --assert(client:send("Client " .. i .. " Send : "))

                   --assert(client:send(receive .. "\n"))
		    print("Receive Client " .. i .. " : " .. _M.receive ) 
		    return _M.receive  
                end
            else
                table.remove(client_tab, i) 
                client:close() 
		conn_count = conn_count  - 1
		_M.receive = nil
                print("Client " .. i .. " disconnect------------------------------------------------------!") 
            end

        end

    end
	--sleep(0.1)
end

function _M.sendData(data)
	for i, client in pairs(client_tab) do 
		--local recvt, sendt, status = socket.select({client}, nil, nil)
		assert(client:send(data .. "\n"))
	end
end

return _M

















