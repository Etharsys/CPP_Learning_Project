# Se familiariser avec l'existant

## Exécution

Compilez et lancez le programme.

Allez dans le fichier `tower_sim.cpp` et recherchez la fonction responsable de gérer les inputs du programme.
Sur quelle touche faut-il appuyer pour ajouter un avion ? La touche c
Comment faire pour quitter le programme ? Les touches q et x
A quoi sert la touche 'F' ? Active/Désactive le fullscreen

Ajoutez un avion à la simulation et attendez.
Que est le comportement de l'avion ? animation de décollage & attérissage :
il attérit, il rend un service, le termine et décolle.
Quelles informations s'affichent dans la console ?
On peut récupérer les actions précédé par l'ID de l'avion :
DL9742 is now landing...
now servicing DL9742...
done servicing DL9742
DL9742 lift off

Ajoutez maintenant quatre avions d'un coup dans la simulation.
Que fait chacun des avions ? Ils tournent en attendant de pouvoir attérir 
(max 3 dans l'aéroport). Puis redécollent pour attendre d'attérir.

## Analyse du code

Listez les classes du programme à la racine du dossier src/.
Pour chacune d'entre elle, expliquez ce qu'elle représente et son rôle dans le programme.
Aircraft -> représente un avion graphique et avec ses donnéss
AirportType -> représente les entrées/sorties d'una aéroport
Airport -> représente un aéroport graphique et avec ses données
Terminal -> représente le terminal / le stop pour que les avions fassent un job
TowerSimulation -> simule le l'aéroport 
Tower -> représente une tour : gestion de l'aéroport : qui peut attérir, décoller !
Waypoint -> représente un point (position) dans l'espace (avec des caractéristiques comme sur le sol, dans un terminal...)

Pour les classes `Tower`, `Aircaft`, `Airport` et `Terminal`, listez leurs fonctions-membre publiques et expliquez précisément à quoi elles servent.
Réalisez ensuite un schéma présentant comment ces différentes classes intéragissent ensemble.
Tower    -> get_instructions    : donne la prochaine instruction à un avion 
            arrived_at_terminal : signale qu'un avion a attérit
Aircraft -> get_flight_num  : récupère le numéro de vol
            distance_to     : donne la distance entre l'avion et un point
            display         : affiche un avion
            move            : déplace l'avion
Airport  -> get_tower : récupère la tour de l'aéroport
            display   : affiche l'aéroport
            move      : déplace tout les terminaux de l'aéroport
Terminal -> in_use         : signal si le terminal est utilisé
            is_servicing   : signal so le terminal produit un service
            assign_craft   : assigne un avion
            start_service  : démarre un service (si avion assez prêt)
            finish_service : arrête un service en cour
            move           : +1 aux progrès de tout les services

Quelles classes et fonctions sont impliquées dans la génération du chemin d'un avion ? la classe aircraft avec les méthodes turn_to_waypoint() et la classe
tower avec les méthodes get_circle et get_instructions.
Quel conteneur de la librairie standard a été choisi pour représenter le chemin ? Point3D (que l'on désigne comme direction)
Expliquez les intérêts de ce choix. Un point paraît peu coûteux pour stocker l'information.

## Bidouillons !

1) Déterminez à quel endroit du code sont définies les vitesses maximales et accélération de chaque avion. Dans la classe aircraft_types
Le Concorde est censé pouvoir voler plus vite que les autres avions.
Modifiez le programme pour tenir compte de cela.
aircraft_types[2] = new AircraftType { .02f, .05f * 2., .02f, MediaPath {    "concorde_af.png" } };

2) Identifiez quelle variable contrôle le framerate de la simulation. Dans le fichier de config : constexpr unsigned int DEFAULT_TICKS_PER_SEC = 16u;
Ajoutez deux nouveaux inputs au programme permettant d'augmenter ou de diminuer cette valeur.
GL::keystrokes.emplace('$', []() { GL::change_ticks(1); });
GL::keystrokes.emplace('*', []() { GL::change_ticks(-1); });
et dans oengl_interface on créer la fonction changeant la variable de tick_speed : inline unsigned int ticks_per_sec = DEFAULT_TICKS_PER_SEC;
(on empêche de passer en dessous de 0)
void change_ticks(int newTick)
{
    if (ticks_per_sec + newTick > 120 || ticks_per_sec + newTick < 1)
    {
        return;
    }
    ticks_per_sec += newTick;
}
Essayez maintenant de mettre en pause le programme en manipulant ce framerate. Que se passe-t-il ? Fixez le problème.
Fallait pas passer dans les négatifs (ou même 0)... mais du coup j'ai déjà fait !

3) Identifiez quelle variable contrôle le temps de débarquement des avions et doublez-le.
Il s'agit de cette variable : constexpr unsigned int SERVICE_CYCLES = 20u;

4) Lorsqu'un avion décolle, celui-ci n'est pas retiré du programme.
Faites en sorte qu'il le soit.
Il faut dans la fonction finish_service() de la classe terminal faire un : delete current_aircraft;

5) Lorsqu'un objet de type `Displayable` est créé, il faut ajouter celui-ci manuellement dans la liste des objets à afficher.
Il faut également penser à le supprimer de cette liste avant de le détruire.
Que pourriez-vous faire afin que l'ajout et la suppression de la liste soit "automatiquement gérée" lorsqu'un `Displayable` est créé ou détruit ?
Faites de même pour `DynamicObject`. //TODO
Il faut ajouter du code dans les constructeur/destructeur des classes `Displayable`.

6) La tour de contrôle a besoin de stocker pour tout `Aircraft` le `Terminal` qui lui est actuellement attribué, afin de pouvoir le libérer une fois que l'avion décolle.
Cette information est actuellement enregistrée dans un `std::vector<std::pair<const Aircraft*, size_t>>` (size_t représentant l'indice du terminal).
Cela fait que la recherche du terminal associé à un avion est réalisée en temps linéaire, par rapport au nombre total de terminaux.
Cela n'est pas grave tant que ce nombre est petit, mais pour préparer l'avenir, on aimerait bien remplacer le vector par un conteneur qui garantira des opérations efficaces, même s'il y a beaucoup de terminaux.\
Modifiez le code afin d'utiliser un conteneur STL plus adapté. Normalement, à la fin, la fonction `find_craft_and_terminal(const Aicraft&)` ne devrait plus être nécessaire.
on peut utiliser un map<const aircraft&, size_t>.
et il existe la méthode find de map qui renvoie un const iterator avec : const auto it = reserved_terminals.find(&aircraft);

## Théorie

1) Comment a-t-on fait pour que seule la classe `Tower` puisse réserver un terminal de l'aéroport ?
Toutes les fonctions de airport sont private (donc inutilisable par les autres classes), mais : friend class Tower;

2) En regardant le contenu de la fonction `void Aircraft::turn(Point3D direction)`, pourquoi selon-vous ne sommes-nous pas passer par une réference ?
?!? pour pas modifier le paramètre direction (par copie, x_x).
Pensez-vous qu'il soit possible d'éviter la copie du `Point3D` passé en paramètre ? oui on peut mais c'est mal, on ne pourra pas mettre le paramètre const
(vu qu'on appelle des fonctions de la classe), donc juste par référence ainsi, on peut appeler turn comme cela : 
target = target - pos - speed;
turn (target);

## Bonus

Le temps qui s'écoule dans la simulation dépend du framerate du programme.
La fonction move() n'utilise pas le vrai temps. Faites en sorte que si.
Par conséquent, lorsque vous augmentez le framerate, la simulation s'exécute plus rapidement, et si vous le diminuez, celle-ci s'exécute plus lentement.

Dans la plupart des jeux ou logiciels que vous utilisez, lorsque le framerate diminue, vous ne le ressentez quasiment pas (en tout cas, tant que celui-ci ne diminue pas trop).
Pour avoir ce type de résultat, les fonctions d'update prennent généralement en paramètre le temps qui s'est écoulé depuis la dernière frame, et l'utilise pour calculer le mouvement des entités.

Recherchez sur Internet comment obtenir le temps courant en C++ et arrangez-vous pour calculer le dt (delta time) qui s'écoule entre deux frames.
Lorsque le programme tourne bien, celui-ci devrait être quasiment égale à 1/framerate.
Cependant, si le programme se met à ramer et que la callback de glutTimerFunc est appelée en retard (oui oui, c'est possible), alors votre dt devrait être supérieur à 1/framerate.

Passez ensuite cette valeur à la fonction `move` des `DynamicObject`, et utilisez-la pour calculer les nouvelles positions de chaque avion.
Vérifiez maintenant en exécutant le programme que, lorsque augmentez le framerate du programme, vous n'augmentez pas la vitesse de la simulation.

Ajoutez ensuite deux nouveaux inputs permettant d'accélérer ou de ralentir la simulation.
