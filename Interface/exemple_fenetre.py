from tkinter import *

fenetre = Tk()
fenetre['bg']='white'
Frame1 = Frame(fenetre, borderwidth=2, relief=GROOVE)
Frame1.pack(side=LEFT, padx=0, pady=0)

Frame2 = Frame(fenetre, borderwidth=2, relief=GROOVE)
Frame2.pack(side=RIGHT, padx=100, pady=10)

canvas = Canvas(Frame1, width=150, height=120, background='white')
ligne1 = canvas.create_line(75, 0, 75, 120)
ligne2 = canvas.create_line(0, 60, 150, 60)
txt = canvas.create_text(75, 60, text="Cible", font="Arial 16 italic", fill="blue")
canvas.pack()

canvas2 = Canvas(Frame2, width=100, height=100, background='white')
ligne12 = canvas2.create_line(75, 0, 75, 120)
ligne22 = canvas2.create_line(0, 60, 150, 60)
txt = canvas2.create_text(75, 60, text="Cible", font="Arial 16 italic", fill="blue")
canvas2.pack()
fenetre.mainloop()