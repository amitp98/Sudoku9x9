#include "generate.c"

void destroy(GtkWidget *widget, gpointer data){
	gtk_main_quit();
}

void difficulty(GtkWidget *widget, int *data){
	int response;
	Sudoku.N = *data;
	Sudoku.N2 = Sudoku.N * Sudoku.N;
	Sudoku.N4 = Sudoku.N2 * Sudoku.N2;
	GtkWidget *dialog = gtk_dialog_new();
	
	GdkWindow *gtk_window = gtk_widget_get_parent_window(widget);
	GtkWindow *parent = NULL;
	gdk_window_get_user_data(gtk_window, (gpointer *)&parent);
		
	gtk_window_set_title(GTK_WINDOW(dialog), "Select Difficulty");
	gtk_window_set_default_size(GTK_WINDOW(dialog), 100, 100);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Easy", 1);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Medium", 2);
	gtk_dialog_add_button(GTK_DIALOG(dialog), "Hard", 3);
	response = gtk_dialog_run(GTK_DIALOG(dialog));

	if (response == 1){
		Sudoku.lev = 1;
		
	}
	if (response == 2){
		Sudoku.lev = 2;
	}
	else if (response == 3){
		Sudoku.lev = 3;
	}
	if(Sudoku.lev != 0){
		gtk_widget_destroy((GtkWidget *)parent);
		gtk_widget_destroy(dialog);
		game(1, NULL);

	}
	
}
void resumeUGame(GtkWidget *widget, GtkWidget *entry){
	GdkWindow *gtk_window = gtk_widget_get_parent_window(widget);
	GtkWindow *parent = NULL;
	gdk_window_get_user_data(gtk_window, (gpointer *)&parent);
	
	char *file, *Filename, *data, c;
	int len, fd, m, i;
	file = (char *)gtk_entry_get_text((GtkEntry *)entry);
	len = strlen(file);
	if(!len){
		GtkWidget *messagedialog;
		gchar *text = "Empty File name";
		messagedialog = gtk_message_dialog_new(GTK_WINDOW(Sudoku.window), 0, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "%s", text);
		gtk_dialog_run(GTK_DIALOG(messagedialog));
		gtk_widget_destroy(messagedialog);
		gtk_widget_destroy((GtkWidget *)parent);		
		return;		
		
	}
	Filename = (char *)malloc(12 + len);
	strcpy(Filename, "./savegame/");	
	strcat(Filename, file);

	fd = open(Filename, O_RDONLY, S_IRUSR | S_IWUSR); 
	if(fd == -1){
		perror(" failed to open file: ");
		GtkWidget *messagedialog;
		gchar *text = "File not found";
		messagedialog = gtk_message_dialog_new(GTK_WINDOW(Sudoku.window), 0, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "%s", text);
		gtk_dialog_run(GTK_DIALOG(messagedialog));
		gtk_widget_destroy(messagedialog);
		gtk_widget_destroy((GtkWidget *)parent);
		
		return;		
	}
	else {
		
		read(fd, &c, 1);
		Sudoku.N = atoi(&c);
		
		Sudoku.N2 = Sudoku.N * Sudoku.N;
		Sudoku.N4 = Sudoku.N2 * Sudoku.N2;
		Sudoku.lev = 0;
		m = 0;
		data = (char *)malloc((Sudoku.N4) + 1);
		for(i = 0; i < Sudoku.N4; i++)
			read(fd, &data[m++], 1);
		data[m] = '\0';
		
		gtk_widget_destroy((GtkWidget *)Sudoku.window);
		game(3, data);
	}
}
void resume( GtkWidget *widget, gpointer data){
	GtkWidget *window2;
	GtkWidget *input;
	GtkWidget *entry;
	GtkBuilder *builder1;

	builder1 = gtk_builder_new();
	gtk_builder_add_from_file (builder1, "load.glade", NULL);

	window2 = GTK_WIDGET(gtk_builder_get_object(builder1, "window1"));
	gtk_builder_connect_signals(builder1, NULL);
	gtk_window_set_title(GTK_WINDOW(window2), "Resume a Sudoku from a File");
	gtk_window_set_default_size(GTK_WINDOW(window2), 300, 100);
	gtk_window_set_position(GTK_WINDOW(window2), GTK_WIN_POS_CENTER);
	g_signal_connect(window2, "destroy", G_CALLBACK(destroyWidget), NULL);

	entry = GTK_WIDGET(gtk_builder_get_object(builder1, "entry1"));

	input = GTK_WIDGET(gtk_builder_get_object(builder1, "load"));
	g_signal_connect(input, "clicked", G_CALLBACK(resumeUGame), entry);
	g_object_unref(builder1);          
	gtk_widget_show_all(window2);
}

void getfile(GtkWidget *fileb, gpointer data1){
	char *file = (char *)malloc(100);
     strcpy(file, gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileb)));
    
     FILE *fd;
	int sz, s, i = 0;
	char data[82];

	fd = fopen(file, "r");
	if(fd == NULL){
		s = 0;
		perror(" failed to open file: ");	
	}
	else{
		fseek(fd, 0, SEEK_END);
		sz = ftell(fd);
	
		fseek(fd, 0, SEEK_SET);
		if(sz != 82)
			s = 0;
		else{
			fread(&data, 81, 1, fd);
			s = 1;
		}
		for(i = 0; i < 81; i++){
				if(!(data[i] >= '0' && data[i] <= '9')){
					s = 0;
					break;
				};
		}
	}
	if(s == 0){
		GtkWidget *messagedialog;
		gchar *text = "Invalid File ";
		messagedialog = gtk_message_dialog_new(GTK_WINDOW(Sudoku.window), 0, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", text);
		gtk_dialog_run(GTK_DIALOG(messagedialog));
		gtk_widget_destroy(messagedialog);
	}
	else{
		Sudoku.N = 3;
		Sudoku.N2 = Sudoku.N * Sudoku.N;
		Sudoku.N4 = Sudoku.N2 * Sudoku.N2;
		Sudoku.lev = 0;
		gtk_widget_destroy((GtkWidget *)Sudoku.window);
		game(3, data);
	}
	
}

int new(){
	int n1 = 3;
	GdkPixbuf *icon;
	icon = gdk_pixbuf_new_from_file("sudoku.png", NULL);
	
	Sudoku.window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	
	gtk_window_set_icon(GTK_WINDOW(Sudoku.window), icon);
	gtk_window_set_title(GTK_WINDOW(Sudoku.window), "SUDOKU");
	gtk_window_set_default_size(GTK_WINDOW(Sudoku.window), 600, 600);
	gtk_window_set_position(GTK_WINDOW(Sudoku.window), GTK_WIN_POS_CENTER);
	g_signal_connect(Sudoku.window, "destroy", G_CALLBACK(destroy), NULL);
	
	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	GtkWidget *buttonbox;
	GtkWidget *button;

	buttonbox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_box_set_spacing(GTK_BOX(buttonbox), 40);
	gtk_box_set_homogeneous (GTK_BOX(buttonbox), TRUE);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(buttonbox), GTK_BUTTONBOX_CENTER);
	gtk_box_pack_start(GTK_BOX(box), buttonbox, TRUE, FALSE, 0);
	
	
	button = gtk_button_new_with_label("New Game");
	gtk_container_add(GTK_CONTAINER(buttonbox), button);
	g_signal_connect (button, "clicked", G_CALLBACK (difficulty), &n1);
	
	
	buttonbox = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
	GtkWidget *label1 = gtk_label_new("---------------------------------------------------------------------- OR ----------------------------------------------------------------------");
	gtk_container_add(GTK_CONTAINER(buttonbox), label1);
	gtk_box_set_spacing(GTK_BOX(buttonbox), 5);
	gtk_box_pack_start(GTK_BOX(box), buttonbox, TRUE, TRUE, 150);
	button = gtk_button_new_with_label("Resume Game");
	gtk_container_add(GTK_CONTAINER(buttonbox), button);
    	g_signal_connect(button, "clicked", G_CALLBACK (resume), NULL);
     
     GtkWidget *label;
	
    	label = gtk_label_new("Select File for user Sudoku");
	gtk_container_add(GTK_CONTAINER(buttonbox), label);
	
     	button = gtk_file_chooser_button_new("Select a File", GTK_FILE_CHOOSER_ACTION_OPEN);
	gtk_container_add(GTK_CONTAINER(buttonbox), button);
	g_signal_connect(button, "file-set", G_CALLBACK(getfile), NULL);
	
	gtk_container_add(GTK_CONTAINER(Sudoku.window), box);
	gtk_widget_show (Sudoku.window);
	gtk_widget_show_all(Sudoku.window);
	gtk_main();

	return 0;	
}
int main(int   argc, char *argv[]){
	gtk_init(&argc, &argv);
	Sudoku.N = 0, Sudoku.N2 = 0, Sudoku.N4 = 0, Sudoku.lev = 0;
	new();
	return 0;
}
