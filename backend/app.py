from flask import Flask, request, jsonify
from flask_cors import CORS
import ctypes
import os

app = Flask(__name__)
CORS(app)

lib = ctypes.CDLL("./report.dll")

class StudentResult(ctypes.Structure):
    _fields_ = [
        ("student_id", ctypes.c_char * 20),
        ("student_name", ctypes.c_char * 50),
        ("percentage", ctypes.c_float),
        ("cgpa", ctypes.c_float)
    ]

class ErrorLog(ctypes.Structure):
    _fields_ = [
        ("message", ctypes.c_char * 200)
    ]

lib.generate_report_data.argtypes = [
    ctypes.c_char_p,
    ctypes.c_int,
    ctypes.POINTER(StudentResult),
    ctypes.POINTER(ErrorLog),
    ctypes.POINTER(ctypes.c_int),
    ctypes.POINTER(ctypes.c_int),
    ctypes.POINTER(ctypes.c_float),
    ctypes.POINTER(ctypes.c_float),
    ctypes.POINTER(ctypes.c_float)
]

lib.generate_report_data.restype = ctypes.c_int

@app.route("/upload", methods=["POST"])
def upload():

    if "file" not in request.files:
        return jsonify({"error": "No file provided"})

    file = request.files["file"]
    N = int(request.form.get("N"))

    if not file.filename.endswith(".txt"):
        return jsonify({"error": "Only .txt files allowed"})

    os.makedirs("uploads", exist_ok=True)
    filepath = os.path.join("uploads", file.filename)
    file.save(filepath)

    MAX = 100

    results = (StudentResult * MAX)()
    errors = (ErrorLog * MAX)()

    valid_count = ctypes.c_int()
    error_count = ctypes.c_int()
    avg = ctypes.c_float()
    high = ctypes.c_float()
    low = ctypes.c_float()

    status = lib.generate_report_data(
        filepath.encode(),N,
        results,
        errors,
        ctypes.byref(valid_count),
        ctypes.byref(error_count),
        ctypes.byref(avg),
        ctypes.byref(high),
        ctypes.byref(low)
    )

    if status == 0:
        return jsonify({"error": "Processing failed"})

    students = []
    for i in range(valid_count.value):
        students.append({
            "student_id": results[i].student_id.decode(),
            "student_name": results[i].student_name.decode(),
            "percentage": round(results[i].percentage, 2),
            "cgpa": round(results[i].cgpa, 2)
        })

    error_list = []
    for i in range(error_count.value):
        error_list.append(errors[i].message.decode())

    return jsonify({
        "students": students,
        "errors": error_list,
        "class_average": round(avg.value, 2),
        "highest": round(high.value, 2),
        "lowest": round(low.value, 2)
    })

if __name__ == "__main__":
    app.run(debug=True)

