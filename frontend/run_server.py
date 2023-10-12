"""
Server for the front end of the application.
This server get the data from the client, save the file, render the data by
calling the command `../build/main filename` and send the result to the client.
"""
import os
import time
import json
import tornado.web
import tornado.ioloop

TEMP_NMN_PATH = os.path.join(os.path.dirname(__file__), "static", "tmp", "nmn")
TEMP_PDF_PATH = os.path.join(os.path.dirname(__file__), "static", "tmp", "pdf")
STATIC_REL_PDF_PATH = os.path.join("tmp", "pdf")

class StaticHandler(tornado.web.StaticFileHandler):
    def parse_url_path(self, url_path):
        return url_path
    
class ReceiveHandler(tornado.web.RequestHandler):
    def post(self):
        # Get the data from the client
        json_data = self.request.body.decode("utf-8")
        # Parse json_data to dict
        data = json.loads(json_data)["text"]
        # Get the ip of the client
        ip = self.request.remote_ip
        # Save the data into a file
        filename = f"{time.strftime('%Y%m%d_%H%M%S')}_{ip}"
        print(TEMP_NMN_PATH)
        mnm_filename = os.path.join(TEMP_NMN_PATH, filename + ".mnm")
        tex_filename = os.path.join(TEMP_PDF_PATH, filename + ".tex")
        pdf_filename = os.path.join(STATIC_REL_PDF_PATH, filename + ".pdf")
        with open(mnm_filename, "w") as f:
            f.write(data)
        # Call the command `../build/main filename`
        exec = os.path.join(os.path.dirname(__file__), "../build/main")
        os.system(f"{exec} {mnm_filename} {tex_filename} {TEMP_PDF_PATH}" )
        # Send the pdf_filename to the client
        self.write(json.dumps({"pdf_path": pdf_filename}))

def make_app(static_path):
    return tornado.web.Application([
        (r"/api/receive", ReceiveHandler),
        (r"/(.*)", StaticHandler, {"path": static_path}), 
    ])

if __name__ == "__main__":
    html_path = os.path.join(os.path.dirname(__file__), "static")
    app = make_app(html_path)
    app.listen(8888)
    tornado.ioloop.IOLoop.current().start()