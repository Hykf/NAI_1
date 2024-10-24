#Zasady: https://docs.google.com/spreadsheets/d/1yJF8SJIqf_2GHLcLyx-9ye85514xuBA7_wA-1tirqL0/edit?gid=0#gid=0

#Autorzy: Mateusz Wróbel & Paweł Mechliński

#Przygotowanie: [Opcjonalnie] jeśli gracz ma zaczynać rozgrywkę to można uruchomić program z flaga -p

![image](https://github.com/user-attachments/assets/e171b8f0-8e75-462c-b45e-423a95cd9b5c)

![image](https://github.com/user-attachments/assets/f3307b55-2cdc-4568-80ba-3ca14ee918ff)

![image](https://github.com/user-attachments/assets/0d1a4f6b-dc76-4934-bad4-70f97edfd7c3)

Gameplay: W grze Connect Four gracz rywalizuje z AI, próbując ułożyć cztery pionki w jednej linii na planszy o wymiarach 7x6. Każdy z graczy na zmianę wrzuca pionek do wybranego przez siebie kolumny, a pionek spada na najniższe wolne pole. Aby wygrać, należy ułożyć swoje pionki w linii poziomej, pionowej lub ukośnej. Gra kończy się, gdy jeden z graczy ułoży cztery pionki w linii.


![image](https://github.com/user-attachments/assets/ac5a82fb-3a86-42e3-b577-ecfeadb365d4)

Implementacja Sztucznej Inteligencji: Gracz komputerowy wybiera optymalny ruch analizując możliwości dla danego układu planszy za pomocą algorytmu mini-max z pruningiem alpha-beta: analizowane są poszczególne możliwości rozwoju rozgrywki, którym przypisywana jest punktacja. Domyślną głębokością analizy rekursywnej jest 5.  