from tkinter import * 
import random

fenetre = Tk()
label = Label(fenetre, text="Hello World")
for ligne in range(5):
    for colonne in range(5):
        Grid.rowconfigure(fenetre,ligne, weight=1)
        Grid.columnconfigure(fenetre,colonne, weight=1)
        #Button(fenetre, text='L%s-C%s' % (ligne, colonne), borderwidth=1).grid(row=ligne, column=colonne, sticky="nsew")
        rand = random.choice(['white', 'black', 'red', 'blue', 'yellow', 'green'])
        Canvas(fenetre, width=150, height=120, background=rand, bd=0, highlightthickness=0, relief='ridge').grid(row=ligne, column=colonne, sticky="nsew")
fenetre.mainloop()
