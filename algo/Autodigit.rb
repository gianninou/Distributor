class Autodigit

	@@opps=["","+","-","*","/","**"]

	def set_number(number)
		@number = number
		@str_number = number.to_s.reverse
		@str_use = Array.new(@str_number.length,false)
		@valid = []
	end


	def compute()
		stack=[]
		def computeRec(rank,stack)

			for i in 0..@str_number.length-1
				if(!@str_use[i])
					stack.push(@str_number[i])
					@str_use[i]=true
					for opp in @@opps
						if(rank>0)
							#on fait chaque opp
							stack.push(opp)
							if(computeRec(rank-1,stack))
								return true
							end
							stack.pop()
							
						else
							#on verifie le rsultat ...
							
							res=false
							calc=0
							begin
								#puts "test de : '#{stack.join("")}'\n"
								calc = eval(stack.join(""))
								res = calc==@number && stack.join("")!=@number.to_s
							rescue SyntaxError => se
							rescue
								#puts "erreur : #{stack.join("")}"
							end
							if(res)
								@valid.push(stack.join())
							end
							stack.pop()
							@str_use[i]=false
							return false
						end
						
					end
					stack.pop()
					@str_use[i]=false
				end
			end
			return false
		end
		res = computeRec(@str_number.length-1,stack)
		#stack.push("-")
		#res = computeRec(@str_number.length-1,stack)
		if(@valid.length>0)
			puts "stack pour #{@number} : #{@valid}"
		end
	end

end


puts "Début du programme"
autodigit = Autodigit.new
for i in 9996..9996
	autodigit.set_number(i)
	autodigit.compute()
end
puts "Fin du programme"