import os
str=input().strip()
with open(str,'r') as f:
    from PIL import Image, ImageFont, ImageDraw
    text=f.read()
    im=Image.new("RGB",(1080,50),(0,0,0))
    dr=ImageDraw.Draw(im)
    font=ImageFont.truetype(os.path.join("fonts", "simsun.ttc"), 18)
    dr.text((5,5),text,font=font,fill="#FFFFFF")
    im.save(str+".png")
