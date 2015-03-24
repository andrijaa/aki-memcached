from array import *
import bmemcached
import pickle

client = bmemcached.Client(('127.0.0.1:11211' ))
client.set('TestKey', 'TestValue')
client.set('Blah', 'BlahValue')
client.set('Cookie', 'Ilike')

client.set('Number', 13)
client.set('Array', array('l', [1, 2, 3, 4, 5]))

print client.get('TestKey')
print client.get('Blah')
print client.get('Cookie')
print client.get('Cookie_NotFound')

print client.get('Number')

intArr = client.get('Array')

print ', '.join( map(str, intArr ))
