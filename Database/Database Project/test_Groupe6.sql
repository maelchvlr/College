/*
LAZRAK Ilona - 22001576
CHARREAU Léo - 21913600
CHEVALIER Maël - 22003139
BARRERE Loreena - 22206698
*/
SET SERVEROUTPUT ON

prompt -- Test du trigger sur l'âge d'une personne : 
prompt -- Quand l'âge est trop grand

INSERT INTO PERSONNE VALUES ('9999999999','TEST_ERREUR','trigger','MTP','9999999999','01-01-1800');

prompt -- Quand l'âge est trop petit

INSERT INTO PERSONNE VALUES ('9999999999','TEST_ERREUR','trigger','MTP','9999999999','01-01-2200');

prompt --
prompt -- Test du trigger sur le délais d'une candidature.
prompt -- Après insertion d'une nouvelle candidature (datant d'un moins ou moins), la table entière est scannee et les candidatures de plus d'un mois en traitement sont automatiquement acceptees

INSERT INTO CANDIDATURE VALUES('0000000009', 'MAS0000010', '04-06-2022', 'TRAITEMENT');
SELECT * FROM CANDIDATURE WHERE NUM_ETUDIANT='0000000009' AND NUM_MASTER='MAS0000010';

prompt --
prompt -- Test de la fonction qui prend en paramètre l'ID d'un master et renvoie le nombre de candidature reçues par celui-ci. Prenons le master d'ID MAS0000001

DECLARE
    res1 NUMBER;
BEGIN
    res1:=nbr_total_cand_par_master('MAS0000001');
    DBMS_OUTPUT.put_line('Donc le master MAS0000001 a recu en tout '|| res1 || ' candidatures.');
END;
/

prompt -- Test de la fonction qui renvoie le nombre de candidature acceptées cette fois-ci. Toujours avec le même master ça donne :

DECLARE
    res2 NUMBER;
BEGIN
    res2:=nbr_cand_accepte_par_master('MAS0000001');
    DBMS_OUTPUT.put_line('le master MAS0000001 a accepte en tout '|| res2 || ' candidature.');
END;
/

prompt -- Test de la fonction pourcentage basique. Ici on va regarder le pourcentage de candidatures acceptées, toujours avec le même master :

DECLARE
    res3 NUMBER;
    res1 NUMBER;
    res2 NUMBER;
BEGIN
    res2:=nbr_cand_accepte_par_master('MAS0000001');
    res1:=nbr_total_cand_par_master('MAS0000001');
    res3:=pourcentage(res2,res1);
    DBMS_OUTPUT.put_line('Donc le master MAS0000001 a accepte '|| res3 || '% de candidatures.');
END;
/

prompt -- 
prompt -- Test de la procédure. On va chercher les statistiques de l'UM fictive de notre BDD.

EXEC statistique_uni('UM')
