import tkinter
from tkinter import*
from socket import *
import sys
from time import sleep
from random import*



root = tkinter.Tk()
root.title("QuizGame")
root.geometry("600x500")
root.config(background="#ffffff")

imgstart=PhotoImage(file="start.png")
imgstart1=PhotoImage(file="start1.png")
imgmenu=PhotoImage(file="btncreate.png")
imgmenu1=PhotoImage(file="btnplay.png")
imgmyown=PhotoImage(file="btnmyown.png")
imgrdy=PhotoImage(file="btnrdy.png")
imgback=PhotoImage(file="back.png")
imgmusic=PhotoImage(file="music.png")
imgsport=PhotoImage(file="sport.png")
imghistory=PhotoImage(file="history.png")
imgmath=PhotoImage(file="math.png")
imgback2=PhotoImage(file="back2.png")
nextque=PhotoImage(file="nextque.png")
quizrdy=PhotoImage(file="quizrdy.png")
btnback3=PhotoImage(file="back3.png")
dellobby=PhotoImage(file="dellobby.png")
a=PhotoImage(file="a.png")
b=PhotoImage(file="b.png")
c=PhotoImage(file="c.png")
d=PhotoImage(file="d.png")
ap=PhotoImage(file="aplay.png")
bp=PhotoImage(file="bplay.png")
cp=PhotoImage(file="cplay.png")
dp=PhotoImage(file="dplay.png")
dalej=PhotoImage(file="dalej.png")
correct=PhotoImage(file="correct.png")
wrong=PhotoImage(file="wrong.png")
img=[imgstart, imgstart1,imgmenu, imgmenu1, imgmyown, imgrdy, imgback, imgmusic,imgsport,imghistory,imgmath,imgback2, nextque,quizrdy,btnback3, dellobby, a, b, c, d, ap, bp, cp, dp, dalej,correct,wrong]

def MessLen(a):
    a=str(a)
    if(len(a)==1):
        a="000"+a
        return a
    elif(len(a)==2):
        a="00"+a
        return a
    elif (len(a) == 3):
        a = "0" + a
        return a
    else:
        return a

def StartMenu(img):

    def StartDel():
        labelimage.destroy(),
        entry.destroy(),
        nickbutton.destroy(),
        labeltext.destroy()

    labelimage=Label(
        root,
        image=img[0],
        background="#ffffff"
    )
    labelimage.pack()

    entry=Entry(
        root, width=35
    )
    entry.pack(pady=(50,0))
    entry.insert(0, "Nickname")

    def Nickname():
        nickname=entry.get()
        print(nickname)
        message="1"+MessLen(len(nickname))+nickname
        try:
            sock.send(message.encode('utf-8'))
        except:
            print("Błąd wysyłania")
        data=''
        data = sock.recv(4).decode('utf-8')
        a=int(data)
        data=''
        data = sock.recv(a).decode('utf-8')
        check=data
        print(check)
        if(check=="no"):
            labeltext.config(text="Nickname zajęty! Wprowadź inny!")
            return
        else:
            id=12
            StartDel()
            ChooseMenu(img, id)

    nickbutton=Button(
        root,
        image=img[1],
        command=Nickname
    )
    nickbutton.pack()

    labeltext = Label(
            root,
            text="",
            font=("Calibri", 16, "bold"),
            background="#ffffff"
        )
    labeltext.pack(pady=(30,0))

def ChooseMenu(img, id):

    def MenuDel():
        btncreate.destroy(),
        btnplay.destroy()

    def ChooseCreate():
        MenuDel()
        CreateMenu(img, id)

    def ChoosePlay():
        MenuDel()
        QuizCode(img,id)

    btncreate = Button(
        root,
        image=img[2],
        command=ChooseCreate
    )
    btncreate.pack(pady=(120,0))

    btnplay = Button(
        root,
        image=img[3],
        command=ChoosePlay
        
    )
    btnplay.pack(pady=(20,0))

def CreateMenu(img, id):
    def CreateDel():
        btnmyown.destroy(),
        btnrdy.destroy(),
        btnback.destroy()

    def ChooseMyOwn():
        CreateDel()
        MyOwnQuizTitle(img,id)

    def ChooseRdy():
        CreateDel()
        ReadyQuiz(img,id)

    def ChooseBack():
        CreateDel()
        ChooseMenu(img, id)

    btnmyown = Button(
        root,
        image=img[4],
        command=ChooseMyOwn
    )
    btnmyown.pack(pady=(120,0))

    btnrdy = Button(
        root,
        image=img[5],
        command=ChooseRdy
    )
    btnrdy.pack(pady=(20,0))

    btnback = Button(
        root,
        image=img[6],
        command=ChooseBack
    )
    btnback.pack(pady=(20, 0))

def ReadyQuiz(img,id):
    def ReadyDel():
        btnmusic.destroy(),
        btnsport.destroy(),
        btnhistory.destroy(),
        btnmath.destroy(),
        btnback2.destroy()

    def ChooseLobbyMusic():
        quiz="mu"
        message = "2" + MessLen(len(quiz)) + quiz
        try:
            sock.send(message.encode('utf-8'))
        except:
            print("Błąd wysyłania")
        data = ''
        data = sock.recv(4).decode('utf-8')
        print(data)
        if (data != ''):
            a = int(data)
            data = ''
            data = sock.recv(a).decode('utf-8')
            if(a == 21 and data[-1]=="&"):
                data=data[0:a-1]
            print(data)
            ReadyDel()
            Lobby(img, id, data)
        else:
            ReadyDel()
            ReadyQuiz(img, id)

    def ChooseLobbySport():
        quiz = "sp"
        message = "2" + MessLen(len(quiz)) + quiz
        try:
            sock.send(message.encode('utf-8'))
        except:
            print("Błąd wysyłania")
        data = ''
        data = sock.recv(4).decode('utf-8')
        print(data)
        if (data != ''):
            a = int(data)
            data = ''
            data = sock.recv(a).decode('utf-8')
            if (a == 21 and data[-1]=="&"):
                data = data[0:a-1]
            print(data)
            ReadyDel()
            Lobby(img, id, data)
        else:
            ReadyDel()
            ReadyQuiz(img, id)

    def ChooseLobbyHistory():
        quiz = "hi"
        message = "2" + MessLen(len(quiz)) + quiz
        print(message)
        try:
            sock.send(message.encode('utf-8'))
        except:
            print("Błąd wysyłania")
        data = ''
        data = sock.recv(4).decode('utf-8')
        print(data)
        if (data != ''):
            a = int(data)
            data = ''
            data = sock.recv(a).decode('utf-8')
            if (a == 21 and data[-1]=="&"):
                data = data[0:a-1]
            print(data)
            ReadyDel()
            Lobby(img, id, data)
        else:
            ReadyDel()
            ReadyQuiz(img, id)

    def ChooseLobbyMath():
        quiz = "ma"
        message = "2" + MessLen(len(quiz)) + quiz
        try:
            sock.send(message.encode('utf-8'))
        except:
            print("Błąd wysyłania")
        data = ''
        data = sock.recv(4).decode('utf-8')
        print(data)
        if(data!=''):
            a = int(data)
            data = ''
            data = sock.recv(a).decode('utf-8')
            if (a == 21 and data[-1]=="&"):
                data = data[0:a-1]
            print(data)
            ReadyDel()
            Lobby(img, id, data)
        else:
            ReadyDel()
            ReadyQuiz(img,id)

    def ChooseBack():
        ReadyDel()
        CreateMenu(img, id)

    btnmusic = Button(
        root,
        image=img[7],
        command=ChooseLobbyMusic
    )
    btnmusic.pack(pady=(10, 0))

    btnsport = Button(
        root,
        image=img[8],
        command=ChooseLobbySport
    )
    btnsport.pack(pady=(10, 0))

    btnhistory = Button(
        root,
        image=img[9],
        command=ChooseLobbyHistory
    )
    btnhistory.pack(pady=(10, 0))

    btnmath = Button(
        root,
        image=img[10],
        command=ChooseLobbyMath
    )
    btnmath.pack(pady=(10, 0))

    btnback2 = Button(
        root,
        image=img[11],
        command=ChooseBack
    )
    btnback2.pack(pady=(10, 0))

def MyOwnQuizTitle(img, id):
    def MyOwnDel():
        title.destroy()
        intitle.destroy()
        button.destroy()
        btnback.destroy()

    def ChooseQue():
        quiztitle = intitle.get()
        message = "3" + MessLen(len(quiztitle)) + quiztitle
        print(message)
        try:
            sock.send(message.encode('utf-8'))
        except:
            print("Błąd wysyłania")
        data = ''
        data = sock.recv(4).decode('utf-8')
        if (data != ''):
            a = int(data)
            data = ''
            data = sock.recv(a).decode('utf-8')
            if (a == 21 and data[-1] == "&"):
                data = data[0:a - 1]
            print(data)
            MyOwnDel()
            MyOwnQuiz(img, data, 0)
        else:
            MyOwnDel()
            MyOwnQuizTitle(img, id)

    def ChooseBack():
        MyOwnDel()
        CreateMenu(img, id)

    title = Label(
        root,
        text="Tytuł",
        font=("Calibri", 16, "bold"),
        background="#ffffff"
    )
    title.pack(pady=(100,0))

    intitle = Entry(
        root,
        width=20,
        font=("Calibri", 20, "bold")
    )
    intitle.pack()

    button = Button(
        root,
        image=img[24],
        command=ChooseQue
    )
    button.pack(pady=(20,0))

    btnback = Button(
        root,
        image=img[11],
        command=ChooseBack
    )
    btnback.pack(pady=(10, 0))

def MyOwnQuiz(img,id,q):
    def MyOwnDel():
        title.destroy()
        question.destroy()
        que.destroy()
        correct.destroy()
        correctanswer.destroy()
        answer2.destroy()
        ans2.destroy()
        answer3.destroy()
        ans3.destroy()
        answer4.destroy()
        ans4.destroy()
        btnnext.destroy()
        btnquizrdy.destroy()
        btnback.destroy()


    def ChooseReady(id,q):
        q+=1
        question = id+";"+str(q)+";"+que.get() + ";" + correctanswer.get() + ";" + ans2.get() + ";" + ans3.get() + ";" + ans4.get()
        message = "5" + MessLen(len(question)) + question
        print(message)
        try:
            sock.send(message.encode('utf-8'))
        except:
            print("Błąd wysyłania")
        data = ''
        data = sock.recv(4).decode('utf-8')
        if (data != ''):
            a = int(data)
            data = ''
            data = sock.recv(a).decode('utf-8')
            if (a == 21 and data[-1] == "&"):
                data = data[0:a - 1]
            print(data)
            MyOwnDel()
            Lobby(img, id, data)
        else:
            MyOwnDel()
            MyOwnQuiz(img, id, q)

    def ChooseNextQue(id, q):
        q+=1
        question=id+";"+str(q)+";"+que.get()+";"+correctanswer.get()+";"+ans2.get()+";"+ans3.get()+";"+ans4.get()
        message = "4" + MessLen(len(question)) + question
        print(message)
        try:
            sock.send(message.encode('utf-8'))
        except:
            print("Błąd wysyłania")
        data = ''
        data = sock.recv(4).decode('utf-8')
        if (data != ''):
            a = int(data)
            data = ''
            data = sock.recv(a).decode('utf-8')
            if (a == 21 and data[-1] == "&"):
                data = data[0:a - 1]
            print(data)
            MyOwnDel()
            MyOwnQuiz(img, id, q)
        else:
            MyOwnDel()
            MyOwnQuiz(img, id, q)

    def ChooseBack():
        MyOwnDel()
        CreateMenu(img, id)

    title = Label(
        root,
        text="Dodaj pytanie!",
        font=("Calibri", 16, "bold"),
        background="#ffffff"
    )
    title.pack(pady=(10, 0))
    question = Label(
        root,
        text="Treść pytania",
        font=("Calibri", 10, "bold"),
        background="#ffffff"
    )
    question.pack(pady=(8, 0))
    que = Entry(
        root, width=60
    )
    que.pack()
    correct = Label(
        root,
        text="Poprawna odpowiedź",
        font=("Calibri", 10, "bold"),
        background="#ffffff"
    )
    correct.pack(pady=(20, 0))
    correctanswer = Entry(
        root, width=40
    )
    correctanswer.pack()
    answer2 = Label(
        root,
        text="Zła odpowiedź 1",
        font=("Calibri", 10, "bold"),
        background="#ffffff"
    )
    answer2.pack(pady=(8, 0))
    ans2 = Entry(
        root, width=40
    )
    ans2.pack()
    answer3 = Label(
        root,
        text="Zła odpowiedź 2",
        font=("Calibri", 10, "bold"),
        background="#ffffff"
    )
    answer3.pack(pady=(8, 0))
    ans3 = Entry(
        root, width=40
    )
    ans3.pack()
    answer4 = Label(
        root,
        text="Zła odpowiedź 3",
        font=("Calibri", 10, "bold"),
        background="#ffffff"
    )
    answer4.pack(pady=(8, 0))
    ans4 = Entry(
        root, width=40
    )
    ans4.pack()
    btnnext = Button(
        root,
        image=img[12],
        command=lambda: ChooseNextQue(id, q)
    )
    btnnext.pack(pady=(10,0),side=LEFT,padx=(70,0))
    btnquizrdy = Button(
        root,
        image=img[13],
        command=lambda: ChooseReady(id, q)
    )
    btnquizrdy.pack(pady=(10,0),side=RIGHT, padx=(0,70))
    btnback = Button(
        root,
        image=img[14],
        command=ChooseBack
    )
    btnback.pack(pady=(10, 0),side=BOTTOM)

def Lobby(img, id, dane):
    waiting=True
    isThree=False
    def LobbyDel():
        title.destroy()
        owner.destroy()
        warning.destroy()
        start.destroy()
        listtitle.destroy()
        partlist.destroy()
        dellobby.destroy()
        quizcode.destroy()
    def DelLobby():
        global waiting
        waiting=False
        mess = code
        message = "f" + MessLen(len(mess)) + mess
        print(message)
        try:
            sock.send(message.encode('utf-8'))
        except:
            print("Błąd wysyłania")
        LobbyDel()
        ChooseMenu(img, id)
    def makeThree(bool):
        global isThree
        isThree=bool

    nickname = dane.split(";")[1]
    quiztitle=dane.split(";")[0]
    code=dane.split(";")[2]
    title = Label(
        root,
        text=quiztitle,
        font=("Calibri", 25, "bold"),
        background="#ffffff"
    )
    title.pack(pady=(10, 0))
    owner = Label(
        root,
        text="Właściciel lobby: "+nickname,
        font=("Calibri", 16, "bold"),
        background="#ffffff"
    )
    owner.pack(pady=(10, 0))
    quizcode = Label(
        root,
        text="KOD: " + code,
        font=("Calibri", 16, "bold"),
        background="#ffffff"
    )
    quizcode.pack(pady=(10, 0))
    warning = Label(
        root,
        text="",
        font=("Calibri", 12, "bold"),
        background="#ffffff"
    )
    warning.pack(pady=(15, 0))
    def Not3Players():
        global isThree
        if(isThree is False):
            warning.config(text="W Lobby musi być przynajmniej 3 graczy, aby rozpocząć grę!")
        else:
            global waiting
            waiting=False
            mess=code
            message = "8" + MessLen(len(mess)) + mess
            print(message)
            try:
                sock.send(message.encode('utf-8'))
            except:
                print("Błąd wysyłania")
            data = ''
            data = sock.recv(4).decode('utf-8')
            if (data != ''):
                a = int(data)
                data = ''
                data = sock.recv(a).decode('utf-8')
                if (a == 21 and data[-1] == "&"):
                    data = data[0:a - 1]
                print(data)
                lp=data.split(";")[0]
                pyt=data.split(";")[1:]
                LobbyDel()
                QuestionOwner(img, code, pyt,lp)
            else:
                LobbyDel()
                Lobby(img, id, dane)
    start = Button(
        root,
        image=img[1],
        command= Not3Players
    )
    start.pack()
    dellobby = Button(
        root,
        image=img[15],
        command=DelLobby
    )
    dellobby.pack(pady=(15, 0))
    listtitle = Label(
        root,
        text="Lista uczestników",
        font=("Calibri", 16, "bold"),
        background="#ffffff"
    )
    listtitle.pack(pady=(10, 0))
    list_part=""
    def listpart():
        print(waiting)
        if waiting:
            mess=code
            message = "7" + MessLen(len(mess)) + mess
            print(message)
            try:
                sock.send(message.encode('utf-8'))
            except:
                print("Błąd wysyłania")
            data = ''
            data = sock.recv(4).decode('utf-8')
            if (data != ''):
                a = int(data)
                data = ''
                data = sock.recv(a).decode('utf-8')
                if (a == 21 and data[-1] == "&"):
                    data = data[0:a - 1]
                print(data)
                if(data=="start"):
                    print("hej")
                else:
                    if(data=="&"):
                        participants=[]
                    else:
                        participants=data.split(";")
                        if(len(participants)>=3):
                            print('more')
                            makeThree(True)
                        else:
                            print('less')
                            makeThree(False)
                    list_part = ""
                    for i in range(len(participants)):
                        if (i % 6 == 0 and i != 0):
                            list_part += str(participants[i]) + "\n"
                        else:
                            list_part += str(participants[i]) + ", "
                    partlist.config(text=list_part)
            else:
                LobbyDel()
                Lobby(img, id, dane)
        partlist.after(1000, listpart)

    partlist = Label(
        root,
        text=list_part,
        font=("Calibri", 10, "bold"),
        background="#ffffff"
    )
    partlist.pack(pady=(10, 0))
    listpart()

def QuizCode(img,id):
    def CodeDel():
        title.destroy()
        quizcode.destroy()
        start.destroy()
        warning.destroy()
        btnback.destroy()
    def ChooseBack():
        CodeDel()
        ChooseMenu(img, id)


    title = Label(
        root,
        text="Podaj kod do gry",
        font=("Calibri", 22, "bold"),
        background="#ffffff"
    )
    title.pack(pady=(140, 0))
    quizcode = Entry(
        root,
        width=10,
        font=("Calibri", 30, "bold"),
    )
    quizcode.pack()
    def CheckCode():
        code=quizcode.get()
        message = "6" + MessLen(len(code)) + code
        print(message)
        try:
            sock.send(message.encode('utf-8'))
        except:
            print("Błąd wysyłania")
        data = ''
        data = sock.recv(4).decode('utf-8')
        if (data != ''):
            a = int(data)
            data = ''
            data = sock.recv(a).decode('utf-8')
            if (a == 21 and data[-1] == "&"):
                data = data[0:a - 1]
            print(data)
            if(data=="no"):
                warning.config(text="Gra o takim kodzie nie istnieje!")
            else:
                CodeDel()
                LobbyPlayer(img, code, data)
        else:
            CodeDel()
            QuizCode(img, id)

    start = Button(
        root,
        image=img[1],
        command=CheckCode
    )
    start.pack(pady=(15,0))
    btnback = Button(
        root,
        image=img[14],
        command=ChooseBack
    )
    btnback.pack(pady=(10, 0))
    warning = Label(
        root,
        text="",
        font=("Calibri", 12, "bold"),
        background="#ffffff"
    )
    warning.pack(pady=(10, 0))

def LobbyPlayer(img, id, dane):
    def LobbyDel():
        title.destroy()
        owner.destroy()
        listtitle.destroy()
        partlist.destroy()
        btnback.destroy()
    def ChooseBack():
        mess = id
        message = "g" + MessLen(len(mess)) + mess
        print(message)
        try:
            sock.send(message.encode('utf-8'))
        except:
            print("Błąd wysyłania")
        LobbyDel()
        ChooseMenu(img, id)
    def ChooseStart():
        LobbyDel()
        QuestionPlayer(img,id)

    nickname = dane.split(";")[1]
    quiztitle=dane.split(";")[0]
    title = Label(
        root,
        text=quiztitle,
        font=("Calibri", 25, "bold"),
        background="#ffffff"
    )
    title.pack(pady=(10, 0))
    owner = Label(
        root,
        text="Właściciel lobby: "+nickname,
        font=("Calibri", 16, "bold"),
        background="#ffffff"
    )
    owner.pack(pady=(10, 0))
    btnback = Button(
        root,
        image=img[14],
        command=ChooseBack
    )
    btnback.pack(pady=(20, 0))
    listtitle = Label(
        root,
        text="Lista uczestników",
        font=("Calibri", 16, "bold"),
        background="#ffffff"
    )
    listtitle.pack(pady=(10, 0))
    def listpart(k):
        if (k==1):
            mess=id
            message = "7" + MessLen(len(mess)) + mess
            print(message)
            try:
                sock.send(message.encode('utf-8'))
            except:
                print("Błąd wysyłania")
            data = ''
            data = sock.recv(4).decode('utf-8')
            if (data != ''):
                a = int(data)
                data = ''
                data = sock.recv(a).decode('utf-8')
                if (a == 21 and data[-1] == "&"):
                    data = data[0:a - 1]
                print(data)
                if(data=="start"):
                    ChooseStart()
                    k=0
                elif (data == "no"):
                    ChooseBack()
                    k = 0
                else:
                    participants=data.split(";")
                    list_part = ""
                    for i in range(len(participants)):
                        if (i % 6 == 0 and i != 0):
                            list_part += str(participants[i]) + "\n"
                        else:
                            list_part += str(participants[i]) + ", "
                    partlist.config(text=list_part)
            else:
                LobbyDel()
                LobbyPlayer(img, id,dane)
        partlist.after(1000, lambda: listpart(k))
    list_part=""
    partlist = Label(
        root,
        text=list_part,
        font=("Calibri", 10, "bold"),
        background="#ffffff"
    )
    partlist.pack(pady=(10, 0))
    listpart(1)

def QuestionOwner(img, id, dane, lp):
    numpyt = dane[0]
    pytanie = dane[1]
    def DelQuestion():
        question.destroy()
        ansa.destroy()
        answera.destroy()
        ansb.destroy()
        answerb.destroy()
        ansc.destroy()
        answerc.destroy()
        ansd.destroy()
        answerd.destroy()
        clock.destroy()
    def ChooseScore():
        mess = id
        message = "b" + MessLen(len(mess)) + mess
        print(message)
        try:
            sock.send(message.encode('utf-8'))
        except:
            print("Błąd wysyłania")
        data = ''
        data = sock.recv(4).decode('utf-8')
        if (data != ''):
            a = int(data)
            data = ''
            data = sock.recv(a).decode('utf-8')
            if (a == 21 and data[-1] == "&"):
                data = data[0:a - 1]
            print(data)
            DelQuestion()
            ScoreBoard(img, id, data, lp, numpyt)
        else:
            DelQuestion()
            QuestionOwner(img, id, dane,lp)

    if(len(pytanie)>70):
        a=len(pytanie)//70
        for i in range(a):
            pytanie=pytanie[0:70*(i+1)]+"-\n"+pytanie[70*(i+1):]
    answers=[dane[2], dane[3], dane[4], dane[5]]
    rand=randint(0,3)
    if(rand==0):
        mess="A"
    elif(rand==1):
        mess="B"
    elif(rand==2):
        mess="C"
    else: mess="D"
    message = "9" + MessLen(len(mess)) + mess
    print(message)
    try:
        sock.send(message.encode('utf-8'))
    except:
        print("Błąd wysyłania")
    x=answers[0]
    answers[0]=answers[rand]
    answers[rand]=x
    question = Label(
        root,
        text="Pytanie "+numpyt+"\n"+pytanie,
        font=("Calibri", 10, "bold"),
        background="#ffffff"
    )
    question.pack()
    answera= Label(
        root,
        image=img[16],
        background="#ffffff"
    )
    answera.pack(pady=(40,0))
    ansa = Label(
        root,
        text=answers[0],
        font=("Calibri", 15, "bold"),
        background="#ffffff"
    )
    ansa.pack()
    answerb = Label(
        root,
        image=img[17],
        background="#ffffff"
    )
    answerb.pack(pady=(20, 0))
    ansb = Label(
        root,
        text=answers[1],
        font=("Calibri", 15, "bold"),
        background="#ffffff"
    )
    ansb.pack()
    answerc = Label(
        root,
        image=img[18],
        background="#ffffff"
    )
    answerc.pack(pady=(20, 0))
    ansc = Label(
        root,
        text=answers[2],
        font=("Calibri", 15, "bold"),
        background="#ffffff"
    )
    ansc.pack()
    answerd = Label(
        root,
        image=img[19],
        background="#ffffff"
    )
    answerd.pack(pady=(20, 0))
    ansd = Label(
        root,
        text=answers[3],
        font=("Calibri", 15, "bold"),
        background="#ffffff"
    )
    ansd.pack()
    def clockcontoler(time):
        if (time>2):
            time-=1
            print(time)
            clock.config(text="Czas: "+str(time-2))
            clock.after(1000, lambda: clockcontoler(time))
        else:
            ChooseScore()

    clock = Label(
        root,
        text="Czas: 30",
        font=("Calibri", 15, "bold"),
        background="#ffffff"
    )
    clock.pack(pady=(10,0))
    clockcontoler(32)

def QuestionPlayer(img, id):
    time=30
    def QueDel():
        question.destroy()
        ansa.destroy()
        ansb.destroy()
        ansc.destroy()
        ansd.destroy()
    def ChooseA():
        global time
        mess = "A"+str(time)
        message = "a" + MessLen(len(mess)) + mess
        print(message)
        try:
            sock.send(message.encode('utf-8'))
        except:
            print("Błąd wysyłania")
        QueDel()
        WaitingRoom(img, id,time)
    def ChooseB():
        global time
        mess = "B" + str(time)
        message = "a" + MessLen(len(mess)) + mess
        print(message)
        try:
            sock.send(message.encode('utf-8'))
        except:
            print("Błąd wysyłania")
        QueDel()
        WaitingRoom(img, id,time)
    def ChooseC():
        global time
        mess = "C" + str(time)
        message = "a" + MessLen(len(mess)) + mess
        print(message)
        try:
            sock.send(message.encode('utf-8'))
        except:
            print("Błąd wysyłania")
        QueDel()
        WaitingRoom(img, id,time)
    def ChooseD():
        global time
        mess = "D" + str(time)
        message = "a" + MessLen(len(mess)) + mess
        print(message)
        try:
            sock.send(message.encode('utf-8'))
        except:
            print("Błąd wysyłania")
        QueDel()
        WaitingRoom(img, id,time)
    def ChooseE():
        global time
        mess = "E" + str(time)
        message = "a" + MessLen(len(mess)) + mess
        print(message)
        try:
            sock.send(message.encode('utf-8'))
        except:
            print("Błąd wysyłania")
        QueDel()
        WaitingRoom(img, id,time)
    tekst="Odpowiedz na pytanie!"
    question = Label(
        root,
        text=tekst,
        font=("Calibri", 20, "bold"),
        background="#ffffff"
    )
    question.pack()
    ansa = Button(
        root,
        image=img[20],
        command=ChooseA
    )
    ansa.pack(pady=(10, 0))
    ansb = Button(
        root,
        image=img[21],
        command=ChooseB
    )
    ansb.pack(pady=(10, 0))
    ansc = Button(
        root,
        image=img[22],
        command=ChooseC
    )
    ansc.pack(pady=(10, 0))
    ansd = Button(
        root,
        image=img[23],
        command=ChooseD
    )
    ansd.pack(pady=(10, 0))
    def clockcontoler(x):
        if (x>0):
            x-=1
            print(x)
            global time
            time=x
            question.after(1000, lambda: clockcontoler(x))
        else:
            ChooseE()
    clockcontoler(30)

def ScoreBoard(img, id, dane, lp, nrp):
    def DelBoard():
        title.destroy()
        place1.destroy()
        place2.destroy()
        place3.destroy()
        place4.destroy()
        place5.destroy()
        btn.destroy()
    def ChooseNext():
        if(nrp==lp):
            mess=id
            message = "f" + MessLen(len(mess)) + mess
            print(message)
            try:
                sock.send(message.encode('utf-8'))
            except:
                print("Błąd wysyłania")
            DelBoard()
            ChooseMenu(img,id)
        else:
            mess = id+nrp
            message = "d" + MessLen(len(mess)) + mess
            print(message)
            try:
                sock.send(message.encode('utf-8'))
            except:
                print("Błąd wysyłania")
            data = ''
            data = sock.recv(4).decode('utf-8')
            if (data != ''):
                a = int(data)
                data = ''
                data = sock.recv(a).decode('utf-8')
                if (a == 21 and data[-1] == "&"):
                    data = data[0:a - 1]
                print(data)
                data=data.split(";")
                DelBoard()
                QuestionOwner(img, id, data, lp)
            else:
                DelBoard()
                ScoreBoard(img, id, dane, lp, nrp)
    if(lp==nrp):
        img1=img[6]
    else:
        img1=img[24]
    table=dane
    table=table.split("^")
    table.sort()
    table.reverse()
    for i in range(len(table)):
        table[i]=table[i].split(";")
    while(len(table)<5):
        table.append(["",""])
    title = Label(
        root,
        text="Tabela wyników",
        font=("Calibri", 25, "bold"),
        background="#ffffff"
    )
    title.pack()
    place1 = Label(
        root,
        text=table[0][1]+" - "+table[0][0],
        font=("Calibri", 35,"bold"),
        fg="#FFD700",
        background="#ffffff"
    )
    place1.pack(pady=(40,0))
    place2 = Label(
        root,
        text=table[1][1] + " - " + table[1][0],
        font=("Calibri", 28, "bold"),
        fg="#808080",
        background="#ffffff"
    )
    place2.pack(pady=(20,0))
    place3 = Label(
        root,
        text=table[2][1] + " - " + table[2][0],
        font=("Calibri", 25, "bold"),
        fg="#CD7F32",
        background="#ffffff"
    )
    place3.pack(pady=(20,0))
    place4 = Label(
        root,
        text=table[3][1] + " - " + table[3][0],
        font=("Calibri", 20, "bold"),
        background="#ffffff"
    )
    place4.pack(pady=(20,0))
    place5 = Label(
        root,
        text=table[4][1] + " - " + table[4][0],
        font=("Calibri", 20, "bold"),
        background="#ffffff"
    )
    place5.pack(pady=(20,0))
    btn = Button(
        root,
        image=img1,
        command=ChooseNext
    )
    btn.pack(pady=(10, 0))

def WaitingRoom(img, id, time):
    def DelRoom():
        douknow.destroy()
        fact1.destroy()
    def ChoosePlayerScore():
        mess = id
        message = "c" + MessLen(len(mess)) + mess
        print(message)
        try:
            sock.send(message.encode('utf-8'))
        except:
            print("Błąd wysyłania")
        data = ''
        data = sock.recv(4).decode('utf-8')
        if (data != ''):
            a = int(data)
            data = ''
            data = sock.recv(a).decode('utf-8')
            if (a == 21 and data[-1] == "&"):
                data = data[0:a - 1]
            print(data)
            DelRoom()
            PlayerScore(img, id, data)
        else:
            DelRoom()
            WaitingRoom(img, id, time)
    time=time+2
    facts=["Spożywanie awokado obniża ryzyko\n chorób serca i spowalnia starzenie","W Łodzi nagrywali teledyski\n m.in. Pitbull i Snoop Dog","Korea Północna i Kuba to jedyne\n miejsca, w których nie można kupić\n Coca-Coli","Francja jest najczęściej odwiedzanym\n krajem na świecie","Hiszpania jest uważana za najbardziej\n górzysty kraj w Europie, a ma ponad\n 8000 kilometrów plaż","Mexico City tonie w tempie 10 cm\n rocznie, czyli 10 razy szybciej\n niż Wenecja","W Niemczech sprzedaje się więcej\n gier planszowych niż gdziekolwiek\n indziej na Ziemi","Hiszpańskie słowo „esposas” oznacza\n zarówno „kajdanki”, jak i „żony”","99% drobnoustrojów żyjących\n w ludziach jest nieznanych nauce","Czarne niedźwiedzie czasami\n hibernują się na szczytach drzew"]
    rand=randint(0,9)
    fact=facts[rand]
    douknow = Label(
        root,
        text="Czy wiesz że?",
        font=("Calibri", 35, "bold"),
        fg="#A020F0",
        background="#ffffff"
    )
    douknow.pack(pady=(100, 0))
    fact1= Label(
        root,
        text=fact,
        font=("Calibri", 20, "bold"),
        background="#ffffff"
    )
    fact1.pack(pady=(60, 0))
    def clockcontoler(x):
        if (x>0):
            x-=1
            print(x)
            global time
            time=x
            fact1.after(1000, lambda: clockcontoler(x))
        else:
            ChoosePlayerScore()
    clockcontoler(time)

def PlayerScore(img,id, dane):
    def DelPS():
        image.destroy()
        tekst.destroy()
    def ChooseNext():
        DelPS()
        QuestionPlayer(img, id)
    def ChooseEnd():
        DelPS()
        EndScreen(img,id,points,place,img1)
    dane=dane.split(";")
    place=dane[1]
    points=dane[2]
    ans=dane[0]
    if(ans=="1"):
        img1=img[25]
    else:
        img1=img[26]
    image = Label(
        root,
        image=img1,
        background="#ffffff"
    )
    image.pack(pady=(20,0))
    tekst = Label(
        root,
        text="Zajmujesz "+place+" miejsce!\nMasz obecnie "+points+" punktów!",
        font=("Calibri", 25, "bold"),
        background="#ffffff"
    )
    tekst.pack(pady=(50,0))
    def check(k):
        if (k==1):
            mess=id
            message = "e" + MessLen(len(mess)) + mess
            print(message)
            try:
                sock.send(message.encode('utf-8'))
            except:
                print("Błąd wysyłania")
            data = ''
            data = sock.recv(4).decode('utf-8')
            if (data != ''):
                a = int(data)
                data = ''
                data = sock.recv(a).decode('utf-8')
                if (a == 21 and data[-1] == "&"):
                    data = data[0:a - 1]
                print(data)
                if(data=="yes"):
                    ChooseNext()
                    k=0
                elif(data=="end"):
                    ChooseEnd()
                    k=0
                print("sadasdas")
            else:
                DelPS()
                PlayerScore(img, id,dane)
            print("aaaaaaa")
        root.after(1000, lambda: check(k))
    check(1)

def EndScreen(img,id, points, place, img1):
    def DelPS():
        image.destroy()
        tekst.destroy()
        btn.destroy()
    def ChooseEnd():
        DelPS()
        ChooseMenu(img,id)
    image = Label(
        root,
        image=img1,
        background="#ffffff"
    )
    image.pack(pady=(20,0))
    tekst = Label(
        root,
        text="Zajmujesz "+place+" miejsce!\nMasz obecnie "+points+" punktów!",
        font=("Calibri", 25, "bold"),
        background="#ffffff"
    )
    tekst.pack(pady=(50,0))
    btn = Button(
        root,
        image=img[6],
        command=ChooseEnd
    )
    btn.pack(pady=(10, 0))

server_address = ('localhost', 5000)
try:
    sock = socket(AF_INET, SOCK_STREAM)
except:
    print("Błąd tworzenia socket!")
try:
    sock.connect(server_address)
except:
    print("Błąd połączenia!")
StartMenu(img)
root.mainloop()
end="0"
try:
    sock.send(end.encode('utf-8'))
except:
    print("Błąd wysłania wiadomości!")
try:
    sock.close()
except:
    print("Błąd zamknięcia połączenia!")