# this executes on load.json & datapack:load
say("Loaded datapack")

# get/reset scoreboard
# stuff set in the load code will be readonly & global unless its something like string or number
# local cache = Scoreboard.Get("cacheBoard", "dummy", true)

# this creates a new datapack:test mcfunction with the contents
# inside compiled to mc commands
function test()	
	# this compiles to "/execute if score test cacheBoard matches 2.. run say Hello, world!"
	if cache.test >= 2 then
		say("Hello, world!")
	end
end