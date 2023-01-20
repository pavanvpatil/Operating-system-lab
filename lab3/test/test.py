fp = open("messages","r")
text = fp.readlines()
for i in text[30489:]:
    print(i)