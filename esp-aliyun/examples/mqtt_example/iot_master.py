from aliyunsdkcore import client
from aliyunsdkiot.request.v20180120 import RegisterDeviceRequest
from aliyunsdkiot.request.v20180120 import PubRequest

accessKeyId = 'LTAIxHn4XXjSkMLj'
accessKeySecret = 'aQcXhDLwkQV9lfGPWqfGzXIIasvhRI'
clt = client.AcsClient(accessKeyId, accessKeySecret, 'cn-shanghai')

request = PubRequest.PubRequest()
request.set_accept_format('json')  
request.set_ProductKey('a1F29g9p65N')
request.set_TopicFullName('/sys/a1F29g9p65N/FD124-CA/thing/service/property/set')  
request.set_MessageContent('aGVsbG8gd29ybGQ=')  
request.set_Qos(1)
result = clt.do_action_with_exception(request)
print 'result : ' + result
