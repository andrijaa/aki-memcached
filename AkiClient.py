import bmemcached
client = bmemcached.Client(('127.0.0.1:11211', ))
client.set('TestKey', 'TestValue')
print client.get('TestKey')
