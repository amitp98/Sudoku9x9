#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "solver.c"

GtkWidget *window1;
GtkGrid *board;
GtkGrid *sud;  
GtkBuilder *builder;
void destroyWidget(GtkWidget *widget, gpointer data){
	gtk_widget_destroy(widget);
}
void on_hint_clicked(GtkWidget *widget, char *data){
	int i, j, k, l, m, c;
	
	m = 0;
	for(i = 0; i < Sudoku.N; i++){
		for(j = 0; j < Sudoku.N; j++){
			for(k = 0; k < Sudoku.N; k++){
				for(l = 0; l < Sudoku.N; l++){
					sud = (GtkGrid *)gtk_grid_get_child_at(board, k, i);
					sud = (GtkGrid *)gtk_grid_get_child_at(sud, l, j);
					
					c =  gtk_entry_get_text_length((GtkEntry *)sud);
					if(!c){
						gtk_entry_set_text((GtkEntry *)sud, &data[m]);
						gtk_entry_grab_focus_without_selecting((GtkEntry *)sud);
						return;
					}
					m++;
				}
			}
		}
	}
}
void on_clear_clicked(GtkWidget *widget, char *data){
	
	int i, j, k, l;
	for(i = 0; i < Sudoku.N; i++){
		for(j = 0; j < Sudoku.N; j++){
			for(k = 0; k < Sudoku.N; k++){
				for(l = 0; l < Sudoku.N; l++){
					sud = (GtkGrid *)gtk_grid_get_child_at(board, k, i);
					sud = (GtkGrid *)gtk_grid_get_child_at(sud, l, j);
					
					if(gtk_editable_get_editable(GTK_EDITABLE(sud)))
						gtk_entry_set_text((GtkEntry *)sud, "");
				}
			}
		}
	}
}

void save(GtkWidget *widget, GtkWidget *entry){

	int len, fd, i, j, k, l;
	char c1 = '0', n, a, b;
	
	GdkWindow *gtk_window = gtk_widget_get_parent_window(widget);
	GtkWindow *parent = NULL;
	gdk_window_get_user_data(gtk_window, (gpointer *)&parent);
	char *file, *Filename;
	file = (char *)gtk_entry_get_text((GtkEntry *)entry);
	len = strlen(file);
	Filename = (char *)malloc(12 + len);
	strcpy(Filename, "./savegame/");	
	strcat(Filename, file);
	
	fd = open(Filename, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR); 
	if(Sudoku.N == 3){
		a = '0';
		b = '9';
		n = Sudoku.N + '0';
	}
	
	if(fd == -1) {
		perror(" failed to open file: ");
		return;		
	}
	else {
		char *c;
		write(fd, &n, 1);	
		for(i = 0; i < Sudoku.N; i++){
			for(j = 0; j < Sudoku.N; j++){
				for(k = 0; k < Sudoku.N; k++){
					for(l = 0; l < Sudoku.N; l++){
						
						sud = (GtkGrid *)gtk_grid_get_child_at(board, k, i);
						sud = (GtkGrid *)gtk_grid_get_child_at(sud, l, j);
						c = (char *) gtk_entry_get_text((GtkEntry *)sud);
						if(c[0] > a && c[0] <= b)	
							write(fd, &c[0], 1);
						else
							write(fd, &c1, 1);
						
					}
				}
			}
		}
		
	}
	free(Filename);
	close(fd);
	gtk_widget_destroy((GtkWidget *)parent);
    	return;
}
void on_save_clicked(GtkWidget *widget, gpointer data){
	GtkWidget *window2;
	GtkWidget *saveb;
	GtkWidget *entry;
	GtkBuilder *builder1;

	builder1 = gtk_builder_new();
	gtk_builder_add_from_file (builder1, "save.glade", NULL);

	window2 = GTK_WIDGET(gtk_builder_get_object(builder1, "window1"));
	gtk_builder_connect_signals(builder1, NULL);
	gtk_window_set_title(GTK_WINDOW(window2), "Save in File");
	gtk_window_set_default_size(GTK_WINDOW(window2), 300, 100);
	gtk_window_set_position(GTK_WINDOW(window2), GTK_WIN_POS_CENTER);
	g_signal_connect(window2, "destroy", G_CALLBACK(destroyWidget), NULL);

	entry = GTK_WIDGET(gtk_builder_get_object(builder1, "entry1"));

	saveb = GTK_WIDGET(gtk_builder_get_object(builder1, "save"));
	g_signal_connect(saveb, "clicked", G_CALLBACK(save), entry);
	
	g_object_unref(builder1);          
	gtk_widget_show_all(window2);
}
void on_verify_clicked(GtkWidget *widget, char *data){

	char *inp = (char *)malloc(Sudoku.N4 + 1);
	char *c, a, b;
	int i, j, k, l, m = 0;
	GtkWidget *messagedialog;
	
	if(Sudoku.N == 3){
		a = '0';
		b = '9';
	}
	
	for(i = 0; i < Sudoku.N; i++){
		for(j = 0; j < Sudoku.N; j++){
			for(k = 0; k < Sudoku.N; k++){
				for(l = 0; l < Sudoku.N; l++){
					sud = (GtkGrid *)gtk_grid_get_child_at(board, k, i);
					sud = (GtkGrid *)gtk_grid_get_child_at(sud, l, j);
					
					c = (char *) gtk_entry_get_text((GtkEntry *)sud);
					if(*c > a && *c <= b){
						inp[m] = *c;
					}
					else{
						gchar *text = "Incorrect Sudoku entries";
						messagedialog = gtk_message_dialog_new(GTK_WINDOW(window1), 0, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "%s", text);
						gtk_dialog_run(GTK_DIALOG(messagedialog));
						gtk_widget_destroy(messagedialog);
						return;
					}
					m++;
				}
			}
		}
	}
	inp[m] = '\0';
	
	if(strcmp(inp, data) == 0){
		gchar *text = "Congratulations! Sudoku solved ";
		messagedialog = gtk_message_dialog_new(GTK_WINDOW(window1), 0, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", text);
		gtk_dialog_run(GTK_DIALOG(messagedialog));
		gtk_widget_destroy(messagedialog);

	}
	else{
		gchar *text = "Incorrect Number";
		messagedialog = gtk_message_dialog_new(GTK_WINDOW(window1), 0, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "%s", text);
		gtk_dialog_run(GTK_DIALOG(messagedialog));
		gtk_widget_destroy(messagedialog);
	}
}

void on_solve_clicked(GtkWidget *widget, char *data){
	
	int m = 0, i, j, k, l;
	for(i = 0; i < Sudoku.N; i++){
		for(j = 0; j < Sudoku.N; j++){
			for(k = 0; k < Sudoku.N; k++){
				for(l = 0; l < Sudoku.N; l++){

					sud = (GtkGrid *)gtk_grid_get_child_at(board, k, i);
					sud = (GtkGrid *)gtk_grid_get_child_at(sud, l, j);
					if(gtk_editable_get_editable(GTK_EDITABLE(sud))){
						
						gtk_entry_set_text((GtkEntry *)sud, &data[m]);
					}
					
					gtk_editable_set_editable(GTK_EDITABLE(sud), FALSE);
					
					m++;
				}
			}
		}
	}
	
}
void on_new_clicked(GtkWidget *widget, gpointer data){
	gtk_widget_destroy((GtkWidget *)data);
	Sudoku.lev = 0, Sudoku.N = 0;
	new();
}

char *generate(int N, int lev){
	char *c;
	FILE *fptr;
	int max, j, n = 0;
	time_t tt;
	srandom(time(&tt));
	if(N == 3){
		c = (char *)malloc(81);
		max = 998;
		if(lev == 1)
			fptr = fopen("./9by9/easy45.txt", "r");
		if(lev == 2)
			fptr = fopen("./9by9/med35.txt", "r");
		if(lev == 3)
			fptr = fopen("./9by9/hard30.txt", "r");
		
	}
	
	
	if (fptr == NULL){
		return NULL;       
	}
	else{
		for(j = 0; j <= n; j++)
			fscanf(fptr, "%s", c);
	}
	
	fclose(fptr);
	
	return c;
}

void game(int a, char *puzzle){
	GtkWidget *save;  
	GtkWidget *sol;  
	GtkWidget *new; 
	GtkWidget *hint; 
	GtkWidget *clear; 
	GtkWidget *verify;  
	GdkPixbuf *icon;
	
	int m = 0, i, j, k, l;
	char *data = (char *)malloc((Sudoku.N4) + 1);

	icon = gdk_pixbuf_new_from_file("sudoku.png", NULL);
	if(a == 1)
		strcpy(data, generate(Sudoku.N, Sudoku.lev));
	if(a == 3)
		data = puzzle;
	
	if(Sudoku.N == 3){
		builder = gtk_builder_new();
		gtk_builder_add_from_file (builder, "sudoku3by3.glade", NULL);

		window1 = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
		gtk_builder_connect_signals(builder, NULL);
		gtk_window_set_icon(GTK_WINDOW(window1), icon);
		gtk_window_set_title(GTK_WINDOW(window1), "SUDOKU 9x9");
		gtk_window_set_default_size(GTK_WINDOW(window1), 240, 450);
		gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
		g_signal_connect(window1, "destroy", G_CALLBACK(destroy), NULL);
	}
	
	
	save = GTK_WIDGET(gtk_builder_get_object(builder, "save"));
	g_signal_connect(save, "clicked", G_CALLBACK(on_save_clicked), NULL);
	
	new = GTK_WIDGET(gtk_builder_get_object(builder, "new"));
	g_signal_connect(new, "clicked", G_CALLBACK(on_new_clicked), window1);
	
	sol = GTK_WIDGET(gtk_builder_get_object(builder, "solve"));
	g_signal_connect(sol, "clicked", G_CALLBACK(on_solve_clicked), data);
	
	verify = GTK_WIDGET(gtk_builder_get_object(builder, "verify"));
	g_signal_connect(verify, "clicked", G_CALLBACK(on_verify_clicked), data);
	
	hint = GTK_WIDGET(gtk_builder_get_object(builder, "hint"));
	g_signal_connect(hint, "clicked", G_CALLBACK(on_hint_clicked), data);
	
	clear = GTK_WIDGET(gtk_builder_get_object(builder, "clear"));
	g_signal_connect(clear, "clicked", G_CALLBACK(on_clear_clicked), data);

	board = (GtkGrid *)GTK_WIDGET(gtk_builder_get_object(builder, "grid1"));
	
	m = 0;
	for(i = 0; i < Sudoku.N; i++){
		for(j = 0; j < Sudoku.N; j++){
			for(k = 0; k < Sudoku.N; k++){
				for(l = 0; l < Sudoku.N; l++){
					sud = (GtkGrid *)gtk_grid_get_child_at(board, k, i);
					sud = (GtkGrid *)gtk_grid_get_child_at(sud, l, j);
					
					if(data[m] > '0' && data[m] <= '9'){
						
						gtk_entry_set_text((GtkEntry *)sud, &data[m]);
						gtk_editable_set_editable(GTK_EDITABLE(sud), FALSE);
						gtk_widget_set_opacity((GtkWidget *)sud, 0.60);
					}
					
					m++;
				}
			}
		}
	}
	data[m] = '\0';
	strcpy(data, solver(data));
	g_object_unref(builder);          
	gtk_widget_show_all(window1);
	gtk_main();
}
