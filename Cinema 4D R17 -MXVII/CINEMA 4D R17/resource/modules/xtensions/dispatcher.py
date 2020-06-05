import socket, ssl, traceback
from c4d.threading import C4DThread

# Thread for forwarding exactly one request.
class DispatcherThread(C4DThread):
		
		useSSL = False
		dispatchSocket = None
		HOST = None
		PORT = None
		METRICS_PORT = None
		METRICS_HOST = None
		
		def Init(self):
				try:
						self.METRICS_HOST = config.GetString(1001)
						self.METRICS_PORT = config.GetInt32(1002)
						self.HOST = ''								 # Symbolic name meaning all available interfaces
						self.PORT = config.GetInt32(1003)	# Arbitrary non-privileged port
						self.useSSL = config.GetBool(1000) # Use SSL?
						self.dispatchSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
						self.dispatchSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
						self.dispatchSocket.bind((self.HOST, self.PORT))
						self.dispatchSocket.listen(1)
						return True
				except Exception, e:
						return False

		def Main(self):
				try:
						conn, addr = self.dispatchSocket.accept()
						try:
								forward = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
								forward.settimeout(5)
								conn.settimeout(5)
								if self.useSSL:
										forward = ssl.wrap_socket(forward)
								forward.connect((self.METRICS_HOST, self.METRICS_PORT))
								# Send request to destination.
								while 1:
										try:
												recv = conn.recv(8192)
												if not recv: break
												sent = 0
												while sent < len(recv):
														if self.useSSL is True:
																sent += forward.write(recv)
														else:
																sent += forward.send(recv)
										except socket.timeout:
												# No more data is going to be sent.
												break
								# Send response to source.
								while 1:
										try:
												if self.useSSL is True:
														fwd = forward.read()
												else:
														fwd = forward.recv(8192)
												if not fwd: break
												sent = 0
												while sent < len(fwd):
														sent += conn.send(fwd)
										except socket.timeout:
												# No more data is going to be sent.
												break
						except Exception, e:
							pass
				except Exception, e:
						pass
				finally:
					forward.close()
					conn.shutdown(socket.SHUT_RDWR)
					conn.close()
					self.dispatchSocket.close()
				return
				
thread = DispatcherThread()	

def Execute():
		if thread.Init() is True:
				thread.Start()
