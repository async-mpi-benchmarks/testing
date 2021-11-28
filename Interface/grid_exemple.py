from tkinter import * 

fenetre = Tk()
label = Label(fenetre, text="Hello World")
for ligne in range(5):
    for colonne in range(5):
        Grid.rowconfigure(fenetre,ligne, weight=1)
        Grid.columnconfigure(fenetre,colonne, weight=1)
        Button(fenetre, text='L%s-C%s' % (ligne, colonne), borderwidth=0).grid(row=ligne, column=colonne, sticky="nsew")

fenetre.mainloop()