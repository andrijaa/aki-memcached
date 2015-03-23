import bmemcached
client = bmemcached.Client(('127.0.0.1:11211' ))
client.set('TestKey', 'TestValue')
client.set('Blah', 'BlahValue')
client.set('Cookie', 'Ilike')

print client.get('TestKey')
print client.get('Blah')
print client.get('Cookie')
