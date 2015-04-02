#!flask/bin/python
from flask import Flask, jsonify, request, abort, Response
import os
import program

app = Flask(__name__)
tasks = []
STARTFLAG = True
Return_M = None
Return_DA = None
Material = None
Menu = None
Type = None
Con = None
By = None
Nowact = None
Nowslot = None
Nowvalue = None

@app.route('/')
def index():
    return "Hello, World!"

@app.route('/getjson', methods=['GET', 'POST'])
def get_json():
    if "application/json" in request.headers["Content-Type"]:
	# Get request info from json
	try:
	    DA = request.json['act']
	except:
	    DA = "none"
	try:
	    Slot = request.json['slot']
	except:
	    Slot = "none"
	try:
	    Value = request.json['value']
	except:
	    Value = "none"

	task = {
	    'DA' : DA,
	    'Slot' : Slot,
	    'Value' : Value
        }

        tasks.append(task)

	# Clear
	print tasks

	# Iner Process
	#program.test()
	#print "Inner Process Clear"
	global STARTFLAG
	if STARTFLAG is True:
	    init_arg1 = "0000000000000000000000000000000000000000000000000000000"
	    init_arg2 = "none"
	    init_arg3 = "none"
	    init_arg4 = "none"
	    init_arg5 = "none"
	    init_arg6 = "none"
	    init_arg7 = "none"
	    init_arg8 = "none"
	    init_arg9 = DA
	    init_arg10 = Slot
	    init_arg11 = Value

	    cmd = './a.out %s %s %s %s %s %s %s %s %s %s %s' % (init_arg1, init_arg2, init_arg3, init_arg4, init_arg5, init_arg6, init_arg7, init_arg8, init_arg9, init_arg10, init_arg11)

	    #TODO Execute Program
	    #print cmd
	    test = os.system(cmd)
 	    STARTFLAG = False
	else:
	    #print "After First Step Process"
	    global Material
	    global Menu
	    global Type
	    global Con
	    global By
	    global Nowact
	    global Nowslot
	    global Nowvalue
	    print DA
	    print Slot
	    print Value
	    cmd = './a.out %s %s %s %s %s %s %s %s %s %s %s' % (Material, Menu, Type, Con, By, Nowact, Nowslot, Nowvalue, DA, Slot, Value)
	    test = os.system(cmd)
	    
	
	global Return_M
	global Return_DA
	#print "Execute Subprogram Clear"
        return Response(Return_M, status=200, mimetype='text/plain')
    else:
	return 'OK'
    
@app.route('/test', methods=['GET'])
def print_json():
    return jsonify({'task':tasks})

@app.route('/getpro', methods=['POST'])
def getpro():
    #print "GetPro Step"
    imd = request.form
    #print imd
    global STARTFLAG
    global Return_M
    global Return_DA
    global Material
    global Menu
    global Type
    global Con
    global By
    global Nowact
    global Nowslot
    global Nowvalue
    Return_M = imd['output_m']
    Return_DA = imd['output_DA']
    Material = imd['Materials']
    Menu = imd['Menu']
    Type = imd['Type']
    Con = imd['Con']
    By = imd['By']
    Nowact = imd['now_act']
    Nowslot = imd['now_slot']
    Nowvalue = imd['now_value']
    #print "==============="
    #print Return_M
    #print Return_DA
    #print Material
    #print Menu
    #print Type
    #print Con
    #print By
    #print Nowact
    #print Nowslot
    #print Nowvalue
    #print "==============="
    if Return_DA == 'end':
	STARTFLAG = True
    return '0K'

if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True, threaded=True)
