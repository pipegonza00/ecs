import client


def main():

    print("Que desea hacer?: ")
    print("op = 1: crear contenedor")
    print("op = 2: Listar contenedores")
    print("op = 3: borrar contenedor")
    print("op = 4: detener contenedor")
    n = int(input("n: "))
    if n == 1:
        name = input("Ingrese nombre de la imagen: ")
        client.crear(name)
    elif n == 2:
        client.listar()
    elif n == 3:
        name = input("Ingrese nombre de la imagen: ")
        client.borrar(name)
    elif n == 4:
        name = input("Ingrese nombre de la imagen: ")
        client.detener(name)

main()