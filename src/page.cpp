const char * html_page = 
"<!DOCTYPE html>\n"
"<html>\n"
"\n"
"<head>\n"
"    <title>ATD3.5-S3 Electronic Shelf Label (ESL) Configs</title>\n"
"    <style>\n"
"        * {\n"
"            box-sizing: border-box;\n"
"        }\n"
"\n"
"        html {\n"
"            height: 100%;\n"
"            display: flex;\n"
"            align-items: center;\n"
"            justify-content: center;\n"
"        }\n"
"\n"
"        body {\n"
"            display: flex;\n"
"            flex-direction: row;\n"
"            align-items: center;\n"
"            justify-content: center;\n"
"            background: rgb(241, 196, 15);\n"
"            background: linear-gradient(126deg, rgba(241, 196, 15, 1) 0%, rgba(46, 204, 113, 1) 100%);\n"
"            font-family: Arial, Helvetica, sans-serif;\n"
"        }\n"
"\n"
"        .main {\n"
"            max-width: 800px;\n"
"            display: flex;\n"
"            flex-direction: row;\n"
"            background-color: #FFF;\n"
"            border-radius: 20px;\n"
"            padding: 30px;\n"
"            box-shadow: 10px 10px 20px rgb(0 0 0 / 9%);\n"
"        }\n"
"\n"
"        .image-preview-box {\n"
"            padding-right: 10px;\n"
"            display: flex;\n"
"            flex-direction: column;\n"
"            align-items: center;\n"
"        }\n"
"\n"
"        .preview {\n"
"            width: 180px;\n"
"            border: 1px solid #e0e0e0;\n"
"            border-radius: 10px;\n"
"            padding: 10px;\n"
"            margin-bottom: 10px;\n"
"        }\n"
"\n"
"        .input-group {\n"
"            min-width: 400px;\n"
"            padding-left: 10px;\n"
"        }\n"
"\n"
"        .input-group>div {\n"
"            margin-bottom: 10px;\n"
"        }\n"
"\n"
"        .input-group>div>label {\n"
"            display: block;\n"
"            margin-bottom: 5px;\n"
"            color: #2c3e50;\n"
"        }\n"
"\n"
"        button {\n"
"            border-radius: 5px;\n"
"            background-color: #2c3e50;\n"
"            color: #FFF;\n"
"            font-size: 14px;\n"
"            border: none;\n"
"            padding: 6px 12px;\n"
"            cursor: pointer;\n"
"        }\n"
"\n"
"        button:hover {\n"
"            background-color: #273746;\n"
"        }\n"
"\n"
"        input {\n"
"            padding: 8px;\n"
"            border-radius: 6px;\n"
"            border: 1px solid #b3b6b7;\n"
"        }\n"
"\n"
"        footer {\n"
"            position: absolute;\n"
"            bottom: 20px;\n"
"            width: 100%;\n"
"            text-align: center;\n"
"            color: #FFF;\n"
"            font-size: 14px;\n"
"        }\n"
"\n"
"        footer a {\n"
"            color: #FFF;\n"
"        }\n"
"\n"
"        #imageInput {\n"
"            display: none;\n"
"        }\n"
"    </style>\n"
"</head>\n"
"\n"
"<body>\n"
"    <section class=\"main\">\n"
"        <div class=\"image-preview-box\">\n"
"            <img class=\"preview\" src=\"\" />\n"
"            <button id=\"select-new-image\">Select</button>\n"
"            <input type=\"file\" id=\"imageInput\" accept=\"image/*\">\n"
"        </div>\n"
"        <div class=\"input-group\">\n"
"            <div>\n"
"                <label for=\"name\">Name</label>\n"
"                <input name=\"name\" id=\"name\" />\n"
"            </div>\n"
"            <div>\n"
"                <label for=\"detail\">Detail</label>\n"
"                <input name=\"detail\" id=\"detail\" style=\"width: 100%;\" />\n"
"            </div>\n"
"            <div>\n"
"                <label for=\"price\">Price</label>\n"
"                <input name=\"price\" id=\"price\" type=\"number\" style=\"width: 60px;\" />\n"
"            </div>\n"
"            <div>\n"
"                <label for=\"code\">Barcode</label>\n"
"                <input name=\"code\" id=\"code\" />\n"
"            </div>\n"
"            <button id=\"save\">Save</button>\n"
"        </div>\n"
"    </section>\n"
"    <footer>Dev by <a href=\"https://www.artronshop.co.th\" target=\"_blank\">ArtronShop</a></footer>\n"
"\n"
"\n"
"    <script>\n"
"        const selectImageBtn = document.querySelector(\"#select-new-image\");\n"
"        const imageInput = document.querySelector(\"#imageInput\");\n"
"        const preview = document.querySelector(\"img.preview\");\n"
"        const canvas = document.createElement(\"canvas\");\n"
"        const saveBtn = document.querySelector(\"#save\");\n"
"        let change_image = false;\n"
"\n"
"        selectImageBtn.addEventListener('click', function () {\n"
"            imageInput.click();\n"
"        });\n"
"\n"
"        imageInput.addEventListener(\"change\", e => {\n"
"            const file = event.target.files[0];\n"
"            if (file) {\n"
"                const reader = new FileReader();\n"
"                reader.onload = function (e) {\n"
"                    const img = new Image();\n"
"                    img.onload = function () {\n"
"                        const maxWidth = 180; // Max Width\n"
"                        const maxHeight = 300; // Max Height\n"
"                        let width = img.width;\n"
"                        let height = img.height;\n"
"\n"
"                        // Calc new image size\n"
"                        if (width > maxWidth || height > maxHeight) {\n"
"                            if (width / height > maxWidth / maxHeight) {\n"
"                                height = (maxWidth / width) * height;\n"
"                                width = maxWidth;\n"
"                            } else {\n"
"                                width = (maxHeight / height) * width;\n"
"                                height = maxHeight;\n"
"                            }\n"
"                        }\n"
"\n"
"                        // Resize iamge with canvas\n"
"                        canvas.width = width;\n"
"                        canvas.height = height;\n"
"                        const ctx = canvas.getContext('2d');\n"
"                        ctx.clearRect(0, 0, canvas.width, canvas.height);\n"
"                        ctx.drawImage(img, 0, 0, width, height);\n"
"\n"
"                        // Convert canvas to Data URL and show image preview\n"
"                        const resizedDataUrl = canvas.toDataURL('image/png');\n"
"                        preview.src = resizedDataUrl;\n"
"                        preview.style.display = 'block';\n"
"                    };\n"
"\n"
"                    img.src = e.target.result;\n"
"                    change_image = true;\n"
"                };\n"
"\n"
"                reader.readAsDataURL(file);\n"
"            }\n"
"        });\n"
"\n"
"        function dataURItoBlob(dataURI) {\n"
"            // convert base64/URLEncoded data component to raw binary data held in a string\n"
"            var byteString;\n"
"            if (dataURI.split(',')[0].indexOf('base64') >= 0)\n"
"                byteString = atob(dataURI.split(',')[1]);\n"
"            else\n"
"                byteString = unescape(dataURI.split(',')[1]);\n"
"\n"
"            // separate out the mime component\n"
"            var mimeString = dataURI.split(',')[0].split(':')[1].split(';')[0];\n"
"\n"
"            // write the bytes of the string to a typed array\n"
"            var ia = new Uint8Array(byteString.length);\n"
"            for (var i = 0; i < byteString.length; i++) {\n"
"                ia[i] = byteString.charCodeAt(i);\n"
"            }\n"
"\n"
"            return new Blob([ia], { type: mimeString });\n"
"        }\n"
"\n"
"        const host = \"http://192.168.68.113\";\n"
"\n"
"        saveBtn.addEventListener('click', function () {\n"
"            let formData = new FormData();\n"
"            for (const key of [\"name\", \"detail\", \"price\", \"code\"]) {\n"
"                formData.append(key, document.querySelector(\"#\" + key).value);\n"
"            }\n"
"            if (change_image) {\n"
"                const blob = dataURItoBlob(preview.src);\n"
"                formData.append(\"image\", blob);\n"
"            }\n"
"\n"
"            fetch(host + \"/save\", {\n"
"                body: formData,\n"
"                method: \"post\"\n"
"            }).then(async () => {\n"
"                window.alert(\"OK !\");\n"
"            });\n"
"        });\n"
"    </script>\n"
"</body>\n"
"\n"
"</html>"
;