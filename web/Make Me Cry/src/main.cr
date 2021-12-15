# A very basic HTTP server
require "http/server"
def normalize(data)
	tmp = ""
	data.each_char do |char|
		if 65374 >= char.ord >= 65281
			tmp += (char.ord - 65248).chr
		elsif 126 >= char.ord >= 33
			tmp += char
		end
	end
	p "Before: " + data + "; After: " + tmp
	return tmp
end
server = HTTP::Server.new do |context|
	context.response.content_type = "text/plain; charset=utf-8"
	if context.request.path == "/flag"
		if context.request.method == "POST"
			begin
				filename = context.request.body.not_nil!.gets_to_end
				filename = File.basename filename
				filename = normalize filename
								if File.match?("*fl*", filename)
					context.response.print "You can't get flag directly"  
				else
					data = File.read(filename)
					context.response.print data
				end    
			rescue
				context.response.print "File not Found"
			end
		else
			context.response.print "你想要 FLAG 吗？尝试向我发送 POST 请求你想要读取的文件名吧，直接发送文件名噢，不需要其他参数。"
		end
	elsif context.request.path == "/"
		dir = Dir.children("/app/").to_s
		context.response.print "你好世界！我做了一个简单的服务器，我经常访问 /flag 路径来读取我需要的文件。这是我当前目录下的文件: " + dir
	end
end
p "Listening"
server.bind_tcp "0.0.0.0", 80
server.listen