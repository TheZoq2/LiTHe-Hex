from chunnel import Socket

socket = Socket('ws://localhost/socket', params={'token': 'blah'})
async with socket:
	channel = socket.channel('room:lobby, {})
	await channel.join()
	incoming = await channel.receive()
	await incoming.reply({'blah': 'whatever'})
	msg = await channel.push('something', {})
	response = await msg.response()


